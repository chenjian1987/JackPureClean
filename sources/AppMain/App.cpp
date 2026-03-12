#include <QApplication>
#include <QIcon>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "MainWindow.h"




int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    // 设置应用程序图标 - 使用绝对路径，尝试PNG和ICO
    QString basePath = QCoreApplication::applicationDirPath() + "/Resource/JackCAD";
    QString pngPath = basePath + ".png";
    QString icoPath = basePath + ".ico";

    qDebug() << "Application directory:" << QCoreApplication::applicationDirPath();
    qDebug() << "PNG path:" << pngPath;
    qDebug() << "ICO path:" << icoPath;
    qDebug() << "PNG exists:" << QFile::exists(pngPath);
    qDebug() << "ICO exists:" << QFile::exists(icoPath);

    // 优先使用PNG，如果失败则使用ICO
    QIcon icon;
    if (QFile::exists(pngPath)) {
        icon = QIcon(pngPath);
        qDebug() << "Loading PNG icon";
    } else if (QFile::exists(icoPath)) {
        icon = QIcon(icoPath);
        qDebug() << "Loading ICO icon";
    }

    qDebug() << "Icon is null:" << icon.isNull();

    if (!icon.isNull()) {
        a.setWindowIcon(icon);
        qDebug() << "Application icon set";
    } else {
        qDebug() << "Failed to load icon";
    }

    MainWindow w;
    w.show();
    return a.exec();
}