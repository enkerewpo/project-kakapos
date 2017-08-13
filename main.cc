#include "Headers/mainwindow.h"
#include "Headers/splashwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    SplashWindow splash;
    w.setStyleSheet("background-color:rgb(65,65,65);");
    splash.setStyleSheet(
                "color:white;"
                "background-color:rgb(57,57,57);");
    splash.show();
    splash.move ((QApplication::desktop()->width() - splash.width())/2,
                 (QApplication::desktop()->height() - splash.height())/2 - 25);
    return a.exec();
}
