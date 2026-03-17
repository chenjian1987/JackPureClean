#pragma once
#include <QFileInfo>
#include <QObject>
#include <QStringList>
#include <mutex>
#include <condition_variable>

class ScannerWorker : public QObject
{
    Q_OBJECT

public:
    explicit ScannerWorker(QObject* parent = nullptr);
    ~ScannerWorker();

    void pause();
    void resume();
    void cancel();

public slots:
    void doScan(const QString& targetDir);
    void doClean(const QStringList& filesToDelete);

signals:
    void fileFound(const QString& name, const QString& path, const QString& time, const QString& size);
    void progressUpdated(int value);
    void statusUpdated(const QString& status);
    void scanFinished(int count, const QString& totalSize);
    void cleanFinished(int successCount, int skipCount);

private:
    QString formatSize(qint64 size);
    void emptyRecycleBin();

    QString normalizePath(const QString& path, bool keepTrailingSlash = false) const;
    QStringList buildScanDirList(const QString& targetDir) const;
    bool waitIfPaused(const QString& pausedText, const QString& resumedText);
    bool isGarbageEntry(const QFileInfo& fileInfo, const QString& lowerTargetDir,  const QString& systemTempDir) const;
    void handleFoundGarbage(const QFileInfo& fileInfo, qint64& totalSizeBytes, int& count);
    void updateScanProgress(int& progressTick);


    QStringList compressDeleteList(const QStringList& inputPaths) const;
    bool forceRemovePath(const QString& path) const;

#ifdef Q_OS_WIN
    void clearReadOnlyAttribute(const QString& path) const;
    void clearReadOnlyAttributeRecursively(const QString& dirPath) const;
#endif

private:
    bool m_isPaused;
    bool m_isCancelled;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};