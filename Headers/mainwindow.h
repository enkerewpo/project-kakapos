#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Headers/highlighter.h"
#include <QMainWindow>
#include <QLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void open_obj();
    void open_obj_file(QString fileName);
    void new_obj();
    void show_about();
    void save_obj();
    void save_as();
    void update();
    void build();
    void run();
    void debug();
    void get_file_dir();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *ButtomHBLayout;
    Highlighter *highlighter;
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
