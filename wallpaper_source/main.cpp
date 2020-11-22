#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QString("TianSong"));
    QCoreApplication::setApplicationName(QString("SimpleDesktop"));

    QSharedMemory sharedMemory(QCoreApplication::organizationName() + QCoreApplication::applicationName());
    if (!sharedMemory.create(512, QSharedMemory::ReadWrite))
    {
        QMessageBox::information(NULL, QStringLiteral("简单桌面"), QStringLiteral("👧 我在屏幕左下角的系统托盘奥"));
        exit(0);
    }

    QTranslator translator;
    translator.load(":/language/qt_zh_CN.qm");
    a.installTranslator(&translator);

    MainWindow w;
    w.hide();

    return a.exec();
}
