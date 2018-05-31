/****************************************************************************
**
** Copyright (C) 2018  Kvar_ispw17
** Contact enkerewpo@gmail.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include/HighlighterC.h"
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
    QString versionTag;

public slots:
    void openObj();
    void openObjFileWithFilename(QString fileName);
    void newObj();
    void showAbout();
    void showLicense();
    void saveObj();
    void saveFileWithFilname(QString fileName);
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
    void dropOnEditor(QString fileName);

private:
    Ui::MainWindow *ui;
    QHBoxLayout *ButtomHBLayout;
    HightlighterC *highlighter;
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
