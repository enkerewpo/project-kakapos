#include "Headers/splashwindow.h"
#include "ui_splashwindow.h"
#include "Headers/mainwindow.h"
#include "Headers/codeeditor.h"
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QSyntaxHighlighter>

SplashWindow::SplashWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SplashWindow) {
    ui->setupUi(this);
    connect(ui->open_object, &QPushButton::clicked, this, &SplashWindow::open_obj);
    connect(ui->new_object, &QPushButton::clicked, this, &SplashWindow::new_obj);
}

SplashWindow::~SplashWindow() {
    delete ui;
}

void SplashWindow::open_obj() {
    QString fileName = \
    QFileDialog::getOpenFileName(this,
                 tr("Open Object"),
                 "",
                  tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
   if(fileName == "") return;
    MainWindow *w = new MainWindow(this);
    close();
    w->show();
    w->move((QApplication::desktop()->width() - w->width())/2,
            (QApplication::desktop()->height() - w->height())/2 - 35);
    w->open_obj_file(fileName);
}

void SplashWindow::new_obj() {
    close();
    MainWindow *w = new MainWindow(this);
    w->show();
    w->move((QApplication::desktop()->width() - w->width())/2,
            (QApplication::desktop()->height() - w->height())/2 - 35);
    w->new_obj();
}
