#include "ScannerWorker.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QThread>
#include <QFile>



ScannerWorker::ScannerWorker(QObject* parent) : QObject(parent), m_isPaused(false), m_isCancelled(false) {}

ScannerWorker::~ScannerWorker() {}



void ScannerWorker::pause() { m_isPaused = true; }

void ScannerWorker::resume()
{
    m_isPaused = false;
    m_cv.notify_one();
}

void ScannerWorker::cancel()
{
    m_isCancelled = true;
    resume();
}

QString ScannerWorker::formatSize(qint64 size)
{
    if (size < 1024) return QString::number(size) + " B";
    if (size < 1024 * 1024) return QString::number(size / 1024.0, 'f', 2) + " KB";
    if (size < 1024 * 1024 * 1024) return QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
    return QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

void ScannerWorker::doScan(const QString& targetDir)
{
    m_isCancelled = false;
    m_isPaused = false;

    QDirIterator it(targetDir,   QDir::Files | QDir::Hidden | QDir::System | QDir::NoSymLinks, QDirIterator::Subdirectories);

    // 将目标目录标准化为小写，并统一使用 Qt 的 '/' 分隔符，防止匹配漏网之鱼
    QString lowerTargetDir = QDir::fromNativeSeparators(targetDir).toLower();

    int count = 0;
    qint64 totalSizeBytes = 0;
    while (it.hasNext())
    {
        if (m_isCancelled) break;

        if (m_isPaused)
        {
            emit statusUpdated(QStringLiteral("当前状态: 已暂停"));
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_isPaused || m_isCancelled; });
            if (m_isCancelled) break;
            emit statusUpdated(QStringLiteral("当前状态: 正在扫描..."));
        }

        it.next();
        QFileInfo fileInfo(it.filePath());

        bool isGarbage = false;
        QString fileName = fileInfo.fileName().toLower();
        QString absPath = fileInfo.absoluteFilePath().toLower();

        // 规则 1：区域性屠杀。只要这个文件在传入的 Temp 目录及其子目录下，无论什么后缀，统统视为垃圾！
        if (absPath.startsWith(lowerTargetDir))
        {
            isGarbage = true;
        }
        // 规则 2：精准打击。如果以后你扫描 C 盘根目录，只要碰到这些后缀，也视为垃圾。 fileName.endsWith(QStringLiteral(".bak")) ||  fileName.endsWith(QStringLiteral(".chk")
        else if (fileName.endsWith(QStringLiteral(".tmp")) ||  fileName.endsWith(QStringLiteral(".log")) )
        {
            isGarbage = true;
        }

        if (isGarbage)
        {
            totalSizeBytes += fileInfo.size();
            emit fileFound(
                fileInfo.fileName(),
                QDir::toNativeSeparators(fileInfo.absoluteFilePath()),
                fileInfo.birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm")), // 加上时分，显得更专业
                formatSize(fileInfo.size())
            );
            count++;
        }
        if (count % 100 == 0) 
        {
            emit progressUpdated((count % 100));
        }
    }

    emit progressUpdated(100);
    emit scanFinished(count, formatSize(totalSizeBytes));
}


void ScannerWorker::doClean(const QStringList& filesToDelete)
{
    emit statusUpdated(QStringLiteral("当前状态: 正在执行物理清理..."));

    int successCount = 0;
    int skipCount = 0;

    // QStringList 完全支持 size() 和 [] 遍历，内部逻辑完全不用动！
    for (int i = 0; i < filesToDelete.size(); ++i)
    {
        QFile file(filesToDelete[i]);
        if (file.remove()) 
        {
            successCount++;
        }
        else 
        {
            skipCount++;
        }

        if (i % 10 == 0 || i == filesToDelete.size() - 1)
        {
            emit progressUpdated(static_cast<int>((i + 1) * 100 / filesToDelete.size()));
        }
    }

    emit cleanFinished(successCount, skipCount);
}