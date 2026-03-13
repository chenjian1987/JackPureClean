#include "ScannerWorker.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QThread>
#include <QFile>
#include <QStandardPaths>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib") 
#endif


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

    // 获取系统 Temp 临时目录 
    QString systemTempDir = QDir::fromNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::TempLocation)).toLower();
    if (!systemTempDir.endsWith(QLatin1Char('/'))) systemTempDir += QLatin1Char('/');

    QString lowerTargetDir = QDir::fromNativeSeparators(targetDir).toLower();
    if (!lowerTargetDir.endsWith(QLatin1Char('/'))) lowerTargetDir += QLatin1Char('/');

    QStringList dirsToScan;
    dirsToScan << targetDir;
    if (lowerTargetDir != systemTempDir) 
    {
        dirsToScan << QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    }
    int count = 0;
    qint64 totalSizeBytes = 0;
    for (const QString& currentScanDir : dirsToScan)
    {
        if (m_isCancelled) break;
        QDirIterator it(currentScanDir, QDir::Files | QDir::Hidden | QDir::System | QDir::NoSymLinks, QDirIterator::Subdirectories);

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

            // 规则 1：区域性屠杀。在目标目录或者系统 Temp 目录下的统统视为垃圾
            if (absPath.startsWith(lowerTargetDir) || absPath.startsWith(systemTempDir))
            {
                isGarbage = true;
            }
            // 规则 2：精准打击。特定后缀的文件
            else if (fileName.endsWith(QStringLiteral(".tmp")) || fileName.endsWith(QStringLiteral(".log")) ||   fileName.endsWith(QStringLiteral(".bak")) || fileName.endsWith(QStringLiteral(".chk")))
            {
                isGarbage = true;
            }

            if (isGarbage)
            {
                totalSizeBytes += fileInfo.size();
                emit fileFound( fileInfo.fileName(), QDir::toNativeSeparators(fileInfo.absoluteFilePath()),   fileInfo.birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm")), formatSize(fileInfo.size())  );
                count++;
            }
            if (count % 10 == 0)
            {
                emit progressUpdated((count / 10) % 100);
            }
        }
    }

    if (!m_isCancelled) 
    {
        emit progressUpdated(100);
        emit scanFinished(count, formatSize(totalSizeBytes));
    }
}

void ScannerWorker::doClean(const QStringList& filesToDelete)
{
    m_isCancelled = false;
    m_isPaused = false;
    emit statusUpdated(QStringLiteral("当前状态: 正在执行物理清理..."));

    int successCount = 0;
    int skipCount = 0;
    int totalFiles = filesToDelete.size();

    for (int i = 0; i < totalFiles; ++i)
    {
        if (m_isCancelled) break;
        if (m_isPaused)
        {
            emit statusUpdated(QStringLiteral("当前状态: 清理已暂停"));
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_isPaused || m_isCancelled; });
            if (m_isCancelled) break;
            emit statusUpdated(QStringLiteral("当前状态: 正在执行物理清理..."));
        }

        QString filePath = filesToDelete[i];
        QFileInfo fileInfo(filePath);
        bool isDeleted = false;
        if (fileInfo.exists())
        {
            if (fileInfo.isDir()) 
            {
                QDir dir(filePath);
                isDeleted = dir.removeRecursively();
            }
            else 
            {
                isDeleted = QFile::remove(filePath); 
            }
        }
        if (isDeleted)
        {
            successCount++;
        }
        else
        {
            skipCount++; 
        }
        if (i % 10 == 0 || i == totalFiles - 1)
        {
            emit progressUpdated(static_cast<int>((i + 1) * 100 / totalFiles));
        }
    }

    if (!m_isCancelled) 
    {
        emptyRecycleBin();
        emit progressUpdated(100);
        emit cleanFinished(successCount, skipCount);
    }
    else 
    {
        emit statusUpdated(QStringLiteral("清理已取消"));
        emit cleanFinished(successCount, skipCount); 
    }
}

void ScannerWorker::emptyRecycleBin()
{
    emit statusUpdated(QStringLiteral("当前状态: 正在清空系统回收站..."));

#ifdef Q_OS_WIN
    // 参数含义:
    // hWnd: 父窗口句柄 (NULL 即可)
    // pszRootPath: 指定盘符的回收站 (NULL 表示清空所有盘符的回收站)
    // dwFlags: 控制标志
    HRESULT result = SHEmptyRecycleBin(
        NULL,
        NULL,
        SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND 
    );

    if (result == S_OK) 
    {
        emit statusUpdated(QStringLiteral("回收站清理完成！"));
    }
    else 
    {
        emit statusUpdated(QStringLiteral("回收站清理失败或已被清空。"));
    }
#else
    emit statusUpdated(QStringLiteral("当前系统不支持原生清理回收站。"));
#endif
}