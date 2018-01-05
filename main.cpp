#include "include/mainwindow.h"
#include "include/splashwindow.h"
#include "include/shadow_effect.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,
                 (QApplication::desktop()->height() - w.height())/2 - 25);
    return a.exec();
}
