#include "Headers/splash_recentfile.h"
#include "ui_splash_recentfile.h"

Splash_RecentFile::Splash_RecentFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Splash_RecentFile)
{
    ui->setupUi(this);
}

Splash_RecentFile::~Splash_RecentFile()
{
    delete ui;
}
