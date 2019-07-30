#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    QTranslator translator_main;
    translator_main.load("./ecd_zh_CN.qm");
    a.installTranslator(&translator_main);
    a.setFont(QFont("Microsoft Yahei", 9));
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();

    return a.exec();
}
