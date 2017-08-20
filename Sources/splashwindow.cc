#include "Headers/splashwindow.h"
#include "ui_splashwindow.h"
#include "Headers/mainwindow.h"
#include "Headers/codeeditor.h"
#include <QFileDialog>
#include <QApplication>
#include <QtMath>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QDesktopWidget>
#include "Headers/customshadoweffect.h"
#include <QSyntaxHighlighter>

SplashWindow::SplashWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SplashWindow) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 80));
//    setWindowIcon(QIcon(""));
//    ui->tableWidget->setAutoFillBackground(true);
//    ui->tableWidget->setGraphicsEffect(bodyShadow);
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
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 80));
    w->setGraphicsEffect(bodyShadow);
    w->show();
    w->move((QApplication::desktop()->width() - w->width())/2,
            (QApplication::desktop()->height() - w->height())/2 - 35);
    w->open_obj_file(fileName);
}

void SplashWindow::new_obj() {
    close();
    MainWindow *w = new MainWindow(this);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 80));
    w->setGraphicsEffect(bodyShadow);
    w->show();
    w->move((QApplication::desktop()->width() - w->width())/2,
            (QApplication::desktop()->height() - w->height())/2 - 35);
    w->new_obj();
}
bool m_Drag;
QPoint m_DragPosition;
void SplashWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
}

void SplashWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
}

void SplashWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_Drag = false;
}
