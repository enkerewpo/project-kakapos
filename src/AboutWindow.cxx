#include "include/AboutWindow.h"
#include "include/headers.h"
#include "ui_AboutWindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    ui->qtVersion->setText(QT_VERSION_STR);
    QDateTime dateTime = QDateTime::currentDateTime(); QString dateTimeString = dateTime.toString();
    ui->Date->setText(dateTimeString);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::receiveVersionTag()
{

}
