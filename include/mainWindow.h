#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include/highlighterC.h"
#include <QMainWindow>
#include <QLayout>
#include <QString>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString tag;

public slots:
    void openObj();
    void openObjFile(QString fileName);
    void newObj();
    void showAbout();
    void showLicense();
    void saveObj();
    void saveFile(QString filename);
    void saveAs();
    void update();
    void build();
    void run();
    void debug();
    void getFileDir();
    void startAstyle();
    void loadSettings();
    void loadPlugin();
    void modified();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *ButtomHBLayout;
    Hightlighter_C *highlighter;
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
