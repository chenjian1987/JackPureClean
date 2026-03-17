#include "ScannerWorker.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QThread>
#include <QFile>
#include <QStandardPaths>
#include <QSet>
#include <algorithm>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#endif





ScannerWorker::ScannerWorker(QObject* parent) : QObject(parent), m_isPaused(false), m_isCancelled(false)
{
}

ScannerWorker::~ScannerWorker()
{
}


void ScannerWorker::doScan(const QString& targetDir)
{
    m_isCancelled = false;
    m_isPaused = false;

    //统一整理路径，转为小写，并且在末尾加斜杠，方便后续判断
    //  注意：用户选择的目录可能是系统 Temp 目录的父级或子级，甚至和系统 Temp 目录完全相同，所以在扫描时都按照规则1当成垃圾来处理
    QString systemTempDir = normalizePath(QStandardPaths::writableLocation(QStandardPaths::TempLocation), true);
    QString lowerTargetDir = normalizePath(targetDir, true);
    //构造扫描目录列表，包含用户选择的目录和系统Temp目录
    QStringList dirsToScan = buildScanDirList(targetDir);

    int count = 0;              //垃圾文件数量
    int progressTick = 0;       //控制进度条刷新频率
	qint64 totalSizeBytes = 0;  //垃圾文件总大小

    for (const QString& currentScanDir : dirsToScan)
    {
        if (m_isCancelled)  break;

		//递归扫描目录下所有条目： AllEntries（包含文件和文件夹） + Hidden（包含隐藏文件） + System（包含系统文件） - NoDotAndDotDot（不包含.和..） - NoSymLinks（不包含符号链接，避免循环扫描）
        QDirIterator it(currentScanDir, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot | QDir::NoSymLinks,  QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            if (m_isCancelled) break;
            if (!waitIfPaused(QStringLiteral("当前状态: 已暂停"), QStringLiteral("当前状态: 正在扫描...")))
            {
                break;
            }
            QString currentPath = it.next();
            QFileInfo fileInfo(currentPath);
			//根据规则判断是否是垃圾条目
            if (isGarbageEntry(fileInfo, lowerTargetDir, systemTempDir))
            {
                //如果是垃圾文件，发信号通知UI，并且累加数量和大小
                handleFoundGarbage(fileInfo, totalSizeBytes, count);
            }
            updateScanProgress(progressTick);
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
	//对删除的列表进行压缩，去掉重复项和被父目录覆盖的子项，减少后续删除操作的数量
    QStringList deleteList = compressDeleteList(filesToDelete);
    int totalFiles = deleteList.size();

    if (totalFiles == 0)
    {
        emit progressUpdated(100);
        emit cleanFinished(0, 0);
        return;
    }

    int successCount = 0;
    int skipCount = 0;

    for (int i = 0; i < totalFiles; ++i)
    {
        if (m_isCancelled)
        {
            break;
        }

        if (!waitIfPaused(QStringLiteral("当前状态: 清理已暂停"),
            QStringLiteral("当前状态: 正在执行物理清理...")))
        {
            break;
        }

        const QString& filePath = deleteList[i];
        bool isDeleted = forceRemovePath(filePath);

        if (isDeleted)
        {
            ++successCount;
        }
        else
        {
            ++skipCount;
        }
		//每删除10个文件或处理到最后一个文件时，更新一次进度
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





void ScannerWorker::pause()
{
    m_isPaused = true;
}

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

// 将字节大小格式化为更友好的字符串
QString ScannerWorker::formatSize(qint64 size)
{
    if (size < 1024) return QString::number(size) + " B";
    if (size < 1024 * 1024) return QString::number(size / 1024.0, 'f', 2) + " KB";
    if (size < 1024 * 1024 * 1024) return QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
    return QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

//把\ 转成/.去掉多余的路径片段，转小写，根据需要保留末尾/
QString ScannerWorker::normalizePath(const QString& path, bool keepTrailingSlash) const
{
    QString result = QDir::fromNativeSeparators(QDir::cleanPath(path)).toLower();
    if (keepTrailingSlash && !result.endsWith('/'))
    {
        result += '/';
    }
    return result;
}

// 构建扫描目录列表，默认一定扫描targetDir,如果targetDir和系统Temp目录不同，则也扫描系统Temp目录
QStringList ScannerWorker::buildScanDirList(const QString& targetDir) const
{
    QStringList dirsToScan;
    QString systemTempDir = normalizePath(QStandardPaths::writableLocation(QStandardPaths::TempLocation), true);
    QString lowerTargetDir = normalizePath(targetDir, true);

    dirsToScan << targetDir;

    if (lowerTargetDir != systemTempDir)
    {
        dirsToScan << QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    }

    return dirsToScan;
}

// 如果当前正在暂停状态，则等待直到恢复或取消，并且在UI上显示相应的状态文本
bool ScannerWorker::waitIfPaused(const QString& pausedText, const QString& resumedText)
{
    if (!m_isPaused)
    {
        return !m_isCancelled;
    }

    emit statusUpdated(pausedText);

    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this] { return !m_isPaused || m_isCancelled; });

    if (m_isCancelled)
    {
        return false;
    }

    emit statusUpdated(resumedText);
    return true;
}

// 根据规则判断是否是垃圾条目：规则1：目标目录和系统 Temp 目录下的内容，统统认为是垃圾；规则2：按后缀识别垃圾文件（.tmp .log .bak .chk）
bool ScannerWorker::isGarbageEntry(const QFileInfo& fileInfo,const QString& lowerTargetDir,const QString& systemTempDir) const
{
    QString fileName = fileInfo.fileName().toLower();
    QString absPath = normalizePath(fileInfo.absoluteFilePath(), fileInfo.isDir());

    // 规则1：目标目录和系统 Temp 目录下的内容，统统认为是垃圾
    if (absPath.startsWith(lowerTargetDir) || absPath.startsWith(systemTempDir))
    {
        return true;
    }
    // 规则2：按后缀识别垃圾文件
    if (!fileInfo.isDir())
    {
        if (fileName.endsWith(QStringLiteral(".tmp")) || fileName.endsWith(QStringLiteral(".log")) ||  fileName.endsWith(QStringLiteral(".bak")) ||   fileName.endsWith(QStringLiteral(".chk")))
        {
            return true;
        }
    }

    return false;
}

//统一处理发现的垃圾条目，发信号通知UI，并且累加数量和大小
void ScannerWorker::handleFoundGarbage(const QFileInfo& fileInfo, qint64& totalSizeBytes, int& count)
{
    QString sizeText;
    if (fileInfo.isDir())
    {
        sizeText = QStringLiteral("文件夹");
    }
    else
    {
        totalSizeBytes += fileInfo.size();
        sizeText = formatSize(fileInfo.size());
    }
    emit fileFound(fileInfo.fileName(),  QDir::toNativeSeparators(fileInfo.absoluteFilePath()),  fileInfo.birthTime().toString(QStringLiteral("yyyy-MM-dd HH:mm")),  sizeText);
    ++count;
}

void ScannerWorker::updateScanProgress(int& progressTick)
{
    ++progressTick;
    if (progressTick % 50 == 0)
    {
        emit progressUpdated((progressTick / 50) % 100);
    }
}

//压缩删除列表，去掉重复项和被父目录覆盖的子项，减少后续删除操作的数量，并且保证删除时父目录在前，子目录在后
QStringList ScannerWorker::compressDeleteList(const QStringList& inputPaths) const
{
    struct PathItem
    {
        QString rawPath;
        QString normalizedPath;
        bool isDir;
    };
    QVector<PathItem> items;
    QSet<QString> uniqueSet;

    for (const QString& path : inputPaths)
    {
        QFileInfo fileInfo(path);
        bool isDir = fileInfo.exists() ? fileInfo.isDir() : false;
        QString normalized = normalizePath(path, isDir);

        if (uniqueSet.contains(normalized)) continue;
        uniqueSet.insert(normalized);
        items.push_back({ path, normalized, isDir });
    }

    // 先按路径长度升序，方便判断是否被父目录覆盖
    std::sort(items.begin(), items.end(),
        [](const PathItem& a, const PathItem& b)
        {
            return a.normalizedPath.length() < b.normalizedPath.length();
        });

    QVector<PathItem> filteredItems;
    for (const PathItem& item : items)
    {
        bool coveredByParent = false;

        for (const PathItem& parent : filteredItems)
        {
            if (!parent.isDir)  continue;    // 只有父项是目录时，才能覆盖其下子项
            QString parentDir = parent.normalizedPath;
            if (!parentDir.endsWith('/'))
            {
                parentDir += '/';
            }
            // 如果当前路径已经包含在某个父目录路径下，则不用重复删
            if (item.normalizedPath.startsWith(parentDir) && item.normalizedPath != parentDir)
            {
                coveredByParent = true;
                break;
            }
        }

        if (!coveredByParent)
        {
            filteredItems.push_back(item);
        }
    }

    // 删除时按深度倒序，优先删除更深层内容
    std::sort(filteredItems.begin(), filteredItems.end(),
        [](const PathItem& a, const PathItem& b)
        {
            return a.normalizedPath.length() > b.normalizedPath.length();
        });

    QStringList result;
    for (const PathItem& item : filteredItems)
    {
        result << item.rawPath;
    }

    return result;
}

bool ScannerWorker::forceRemovePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if (!fileInfo.exists())
    {
        return true;
    }

#ifdef Q_OS_WIN
    if (fileInfo.isDir())
    {
		// Windows系统中，删除文件或文件夹时，如果它们具有只读属性，删除操作会失败，所以先递归清除只读属性
        clearReadOnlyAttributeRecursively(path);
    }
    else
    {
		// 对于单个文件，直接清除只读属性
        clearReadOnlyAttribute(path);
        QFile::setPermissions(path,
            QFileDevice::ReadOwner | QFileDevice::WriteOwner |
            QFileDevice::ReadUser | QFileDevice::WriteUser |
            QFileDevice::ReadGroup | QFileDevice::WriteGroup |
            QFileDevice::ReadOther | QFileDevice::WriteOther);
    }
#endif

    if (fileInfo.isDir())
    {
        QDir dir(path);
        return dir.removeRecursively();
    }

    return QFile::remove(path);
}

void ScannerWorker::emptyRecycleBin()
{
    emit statusUpdated(QStringLiteral("当前状态: 正在清空系统回收站..."));

#ifdef Q_OS_WIN
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





#ifdef Q_OS_WIN
// Windows系统中，删除文件或文件夹时，如果它们具有只读属性，删除操作会失败，所以先递归清除只读属性
void ScannerWorker::clearReadOnlyAttribute(const QString& path) const
{
    LPCWSTR lpPath = reinterpret_cast<LPCWSTR>(path.utf16());
    DWORD attrs = GetFileAttributesW(lpPath);
    if (attrs == INVALID_FILE_ATTRIBUTES)
    {
        return;
    }

    DWORD newAttrs = attrs & ~FILE_ATTRIBUTE_READONLY;
    SetFileAttributesW(lpPath, newAttrs);
}

//  递归清除目录及其下所有内容的只读属性，确保后续删除操作不会因为只读属性而失败
void ScannerWorker::clearReadOnlyAttributeRecursively(const QString& dirPath) const
{
    QDirIterator it(dirPath,
        QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot | QDir::NoSymLinks,
        QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        clearReadOnlyAttribute(it.next());
    }

    clearReadOnlyAttribute(dirPath);
}
#endif