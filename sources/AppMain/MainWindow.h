#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidget>
#include <QLabel>
#include <QThread>
#include <vector>
#include "ScannerWorker.h"
#include <QStringList>



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onMainButtonClicked();
    void onPauseButtonClicked();
    void appendFileInfo(const QString& name, const QString& path, const QString& timeStr, const QString& sizeStr);
    void onScanFinished(int totalFiles, const QString& totalSizeStr);
    void onCleanFinished(int successCount, int skipCount);
    void updateStatus(const QString& status);

private:
    void setupUI();

    QPushButton* btnMain;
    QPushButton* btnPause;
    QProgressBar* progressBar;
    QTableWidget* tableWidget;
    QLabel* lblStatus;
    QLabel* lblSummary;

    enum State { IDLE, SCANNING, SCANNED, CLEANING };
    State m_currentState;
    bool m_isPaused;

    QThread* m_workerThread;
    ScannerWorker* m_worker;
    QStringList m_scannedFiles;
};

#endif // MAINWINDOW_H