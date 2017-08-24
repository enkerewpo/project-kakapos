#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QMainWindow>

namespace Ui {
class SplashWindow;
}

class SplashWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplashWindow(QWidget *parent = 0);
    ~SplashWindow();
public slots:
    void open_obj();
    void new_obj();

private:
    Ui::SplashWindow *ui;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // SPLASHWINDOW_H
