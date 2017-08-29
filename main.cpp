#include "include/mainwindow.h"
#include "include/splashwindow.h"
#include "include/shadow_effect.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    SplashWindow splash;
    splash.setStyleSheet(
                "color:white;"
                "background-color:rgb(57,57,57);");

//    splash.show();
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,
                 (QApplication::desktop()->height() - w.height())/2 - 25);
//    splash.move ((QApplication::desktop()->width() - splash.width())/2,
//                 (QApplication::desktop()->height() - splash.height())/2 - 25);
    return a.exec();
}
