#ifndef SPLASH_RECENTFILE_H
#define SPLASH_RECENTFILE_H

#include <QWidget>

namespace Ui {
class Splash_RecentFile;
}

class Splash_RecentFile : public QWidget
{
    Q_OBJECT

public:
    explicit Splash_RecentFile(QWidget *parent = 0);
    ~Splash_RecentFile();

private:
    Ui::Splash_RecentFile *ui;
};

#endif // SPLASH_RECENTFILE_H
