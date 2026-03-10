#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), m_currentState(IDLE), m_isPaused(false)
{
    setupUI();

    m_workerThread = new QThread(this);
    m_worker = new ScannerWorker();
    m_worker->moveToThread(m_workerThread);

    connect(m_worker, &ScannerWorker::fileFound, this, &MainWindow::appendFileInfo);
    connect(m_worker, &ScannerWorker::progressUpdated, progressBar, &QProgressBar::setValue);
    connect(m_worker, &ScannerWorker::statusUpdated, this, &MainWindow::updateStatus);
    connect(m_worker, &ScannerWorker::scanFinished, this, &MainWindow::onScanFinished);
    connect(m_worker, &ScannerWorker::cleanFinished, this, &MainWindow::onCleanFinished);

    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    m_workerThread->start();
}

MainWindow::~MainWindow()
{
    m_worker->cancel();
    m_workerThread->quit();
    m_workerThread->wait();
}





void MainWindow::setupUI()
{
    // 使用 QStringLiteral 彻底解决中文乱码问题
    this->setWindowTitle(QStringLiteral("垃圾文件清理助手 (JackC Cleaner)"));
    this->resize(800, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // --- 顶部控制区 ---
    QHBoxLayout* controlLayout = new QHBoxLayout();
    btnMain = new QPushButton(QStringLiteral("开始扫描"), this);
    btnMain->setFixedSize(100, 30);

    btnPause = new QPushButton(QStringLiteral("暂停"), this);
    btnPause->setFixedSize(80, 30);
    btnPause->setEnabled(false);

    lblStatus = new QLabel(QStringLiteral("当前状态: 等待中..."), this);

    controlLayout->addWidget(btnMain);
    controlLayout->addSpacing(10);
    controlLayout->addWidget(btnPause);
    controlLayout->addSpacing(30);
    controlLayout->addWidget(lblStatus);
    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);
    mainLayout->addSpacing(10);

    // --- 进度条 ---
    QHBoxLayout* progressLayout = new QHBoxLayout();
    QLabel* lblProgress = new QLabel(QStringLiteral("进度: "), this);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setFixedHeight(18);
    progressLayout->addWidget(lblProgress);
    progressLayout->addWidget(progressBar);
    mainLayout->addLayout(progressLayout);
    mainLayout->addSpacing(10);

    // --- 详细信息列表 ---
    QLabel* lblDetailTitle = new QLabel(QStringLiteral("详细信息:"), this);
    mainLayout->addWidget(lblDetailTitle);

    // 将列数改为 5
    tableWidget = new QTableWidget(0, 5, this);
    tableWidget->setHorizontalHeaderLabels({
        QStringLiteral("名称"),
        QStringLiteral("类型"),   // 新增列
        QStringLiteral("路径"),
        QStringLiteral("创建时间"),
        QStringLiteral("大小")
        });

     // 第 0 列 (名称): 保持 200 像素
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    tableWidget->setColumnWidth(0, 200);

    // 第 1 列 (类型): 保持 80 像素
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->setColumnWidth(1, 80);

    // 第 2 列 (路径): 改为 ResizeToContents (根据内容真实长度自动撑开宽度，绝不省略！)
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    // 第 3、4 列 (时间、大小): 同样根据内容自适应
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // 取消 Qt 默认的文字省略机制 (双重保险)
    tableWidget->setTextElideMode(Qt::ElideNone);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setAlternatingRowColors(true);
    mainLayout->addWidget(tableWidget);
    mainLayout->addSpacing(10);

    // --- 底部汇总区 ---
    lblSummary = new QLabel(QStringLiteral("已发现垃圾文件: 0 个  |  预计释放空间: 0 B"), this);
    mainLayout->addWidget(lblSummary);

    connect(btnMain, &QPushButton::clicked, this, &MainWindow::onMainButtonClicked);
    connect(btnPause, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
}

void MainWindow::onMainButtonClicked()
{
    if (m_currentState == IDLE) {
        m_currentState = SCANNING;
        btnMain->setEnabled(false);
        btnPause->setEnabled(true);
        btnPause->setText(QStringLiteral("暂停"));
        m_isPaused = false;

        tableWidget->setRowCount(0);
        m_scannedFiles.clear();
        progressBar->setValue(0);
        lblStatus->setText(QStringLiteral("当前状态: 正在扫描..."));
        lblSummary->setText(QStringLiteral("已发现垃圾文件: 0 个  |  预计释放空间: 0 B"));

        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QMetaObject::invokeMethod(m_worker, "doScan", Q_ARG(QString, tempPath));

    }
    else if (m_currentState == SCANNED) 
    {
        m_currentState = CLEANING;
        btnMain->setEnabled(false);
        btnPause->setEnabled(false);
        lblStatus->setText(QStringLiteral("当前状态: 正在清理..."));
        progressBar->setValue(0);

        QMetaObject::invokeMethod(m_worker, "doClean", Q_ARG(QStringList, m_scannedFiles));
    }
}

void MainWindow::onPauseButtonClicked()
{
    if (m_currentState == SCANNING) {
        if (!m_isPaused) {
            m_worker->pause();
            btnPause->setText(QStringLiteral("继续"));
            m_isPaused = true;
        }
        else {
            m_worker->resume();
            btnPause->setText(QStringLiteral("暂停"));
            m_isPaused = false;
        }
    }
}

void MainWindow::updateStatus(const QString& status)
{
    lblStatus->setText(status);
}

void MainWindow::appendFileInfo(const QString& name, const QString& path, const QString& timeStr, const QString& sizeStr)
{
    QString typeStr = QStringLiteral("未知文件");
    int dotIndex = name.lastIndexOf('.');
    if (dotIndex != -1 && dotIndex < name.length() - 1) {
        typeStr = name.mid(dotIndex + 1).toUpper() + QStringLiteral(" 文件");
    }

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    // 【关键修改】创建 Item 并为其设置 ToolTip (鼠标悬停提示)
    QTableWidgetItem* nameItem = new QTableWidgetItem(name);
    nameItem->setToolTip(name); // 鼠标悬停显示完整文件名

    QTableWidgetItem* typeItem = new QTableWidgetItem(typeStr);

    QTableWidgetItem* pathItem = new QTableWidgetItem(path);
    pathItem->setToolTip(path); // 鼠标悬停显示完整超长路径

    QTableWidgetItem* timeItem = new QTableWidgetItem(timeStr);
    QTableWidgetItem* sizeItem = new QTableWidgetItem(sizeStr);

    // 填入表格
    tableWidget->setItem(row, 0, nameItem);
    tableWidget->setItem(row, 1, typeItem);
    tableWidget->setItem(row, 2, pathItem);
    tableWidget->setItem(row, 3, timeItem);
    tableWidget->setItem(row, 4, sizeItem);

    m_scannedFiles.append(path);
}

void MainWindow::onScanFinished(int totalFiles, const QString& totalSizeStr)
{
    m_currentState = SCANNED;
    btnMain->setText(QStringLiteral("立即清理"));
    btnMain->setEnabled(true);
    btnPause->setEnabled(false);
    btnPause->setText(QStringLiteral("暂停"));
    lblStatus->setText(QStringLiteral("当前状态: 等待清理..."));
    lblSummary->setText(QString(QStringLiteral("已发现垃圾文件: %1 个  |  预计释放空间: %2")).arg(totalFiles).arg(totalSizeStr));
}

void MainWindow::onCleanFinished(int successCount, int skipCount)
{
    m_currentState = IDLE;
    btnMain->setText(QStringLiteral("开始扫描"));
    btnMain->setEnabled(true);
    tableWidget->setRowCount(0); // 清空表格
    progressBar->setValue(100);
    lblStatus->setText(QStringLiteral("当前状态: 清理完毕"));

    QString resultMsg = QString(QStringLiteral("清理战果: 成功删除 %1 个文件 | 跳过占用文件: %2 个")).arg(successCount).arg(skipCount);
    lblSummary->setText(resultMsg);

    m_scannedFiles.clear();
}