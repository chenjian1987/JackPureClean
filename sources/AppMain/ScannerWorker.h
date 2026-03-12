#ifndef SCANNERWORKER_H
#define SCANNERWORKER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <QStringList>


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
    void emptyRecycleBin();

signals:
    void fileFound(const QString& name, const QString& path, const QString& timeStr, const QString& sizeStr);
    void progressUpdated(int percentage);
    void statusUpdated(const QString& status);
    void scanFinished(int totalFiles, const QString& totalSizeStr);

    void cleanFinished(int successCount, int skipCount);

private:
    std::atomic<bool> m_isPaused;
    std::atomic<bool> m_isCancelled;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    QString formatSize(qint64 size);
};
#endif // SCANNERWORKER_H