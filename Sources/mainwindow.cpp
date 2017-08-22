#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/shadow_effect.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QLayout>
#include <QTime>
#include <QtGlobal>
#include <QDebug>
#include <QWidget>
#include <QScrollBar>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <QTextStream>
#include <yaml-cpp/yaml.h>
#include "Headers/code_editor.h"

CodeEditor *editor;
QDir *pDir;
QString fileDir, tmpfileDir;
QString global_filename, tmp_filename, global_file_shortname, g_filedir;
QString gcc_path;
bool modified, saved, newfile;
int dynamic_width, dynamic_height, user_fontsize;

#ifdef Q_OS_WIN
    int tab_len = 4;
#elif defined (Q_OS_OSX)
    int tab_len = 4;
#else
int tab_len = 4;
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    editor = new CodeEditor(ui->centralWidget);

    setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::open_obj);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::show_about);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save_obj);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::save_as);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::new_obj);
    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::build);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::run);

    connect(ui->actionAStyle, &QAction::triggered, this, &MainWindow::start_astyle);


    editor->setWordWrapMode(QTextOption::NoWrap);
    editor->fontsize = 11;
    editor->verticalScrollBar()->setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                                          "    border: 0px solid #999999;"
                                                          "    background:transparent;"
                                                          "    width:10px;    "
                                                          "    margin: 0px 0px 0px 0px;"
                                                          "}"
                                                          "QScrollBar::handle:vertical {"
                                                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                                          "    stop: 0 white, stop: 0.5 white, stop:1 white);"
                                                          "    min-height: 0px;"
                                                          "}"
                                                          "QScrollBar::add-line:vertical {"
                                                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                                          "    stop: 0 white, stop: 0.5 white,  stop:1 white);"
                                                          "    height: 0px;"
                                                          "    subcontrol-position: bottom;"
                                                          "    subcontrol-origin: margin;"
                                                          "}"
                                                          "QScrollBar::sub-line:vertical {"
                                                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                                          "    stop: 0  white, stop: 0.5 white,  stop:1 white);"
                                                          "    height: 0 px;"
                                                          "    subcontrol-position: top;"
                                                          "    subcontrol-origin: margin;"
                                                          "}"
                                                          ));

    user_fontsize = 11;
    editor->setGeometry(-1, 0, width() + 2, height() - 40);
#ifdef  Q_OS_MAC
    editor->setFont(QFont("DejaVu Sans Mono",user_fontsize + 2));
#elif defined (Q_OS_WIN)
    editor->setFont(QFont("Consolas", user_fontsize));
#endif
    editor->setStyleSheet("color:rgb(235,235,235);");
    editor->setTabStopWidth(fontMetrics().width(QLatin1Char('9')) * tab_len);
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update() {
    this->setWindowTitle(global_filename);
    if(global_filename.endsWith(".cpp", Qt::CaseInsensitive) ||
       global_filename.endsWith(".cc", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".c++", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".hpp", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".c", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".h", Qt::CaseInsensitive)) {
        highlighter = new Hightlighter_C(editor->document());
        editor->do_autotab = true;
        editor->filetype = "cplusplus";
    }
}


void MainWindow::open_obj() {
    QString fileName = \
    QFileDialog::getOpenFileName(this,
                 tr("Open File"),
                 "",
               tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
    pDir = new QDir(".");
    global_filename = fileName;
    get_file_dir();
    update();
    fileDir = pDir->filePath(fileName);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    this->setWindowTitle(global_filename);
    editor->clear();
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
}

void MainWindow::open_obj_file(QString fileName) {
    pDir = new QDir(".");
    global_filename = fileName;
    fileDir = pDir->filePath(fileName);
    get_file_dir();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
    update();

}
void MainWindow::resizeEvent(QResizeEvent *event) {
    dynamic_height = this->height();
    dynamic_width = this->width();
    editor->setGeometry(-1, 0, width() + 2, height() - 20);
}

void MainWindow::new_obj() {
    global_filename = "";
    newfile = true;
    if(modified) {

    }
    editor->clear();
    update();
}

void MainWindow::save_file(QString filename) {
    pDir = new QDir(".");
    fileDir = pDir->filePath(filename);
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << filename;
        out << editor->toPlainText();
    }
}

void MainWindow::save_obj() {
    if(!newfile) {
        pDir = new QDir(".");
        fileDir = pDir->filePath(global_filename);
        QFile file(global_filename);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << global_filename;
            out << editor->toPlainText();
        }

//        QFile file2(global_filename + ".kat");
//        if (file2.open(QIODevice::ReadWrite))
//        {
//            QTextStream out(&file);
//            qDebug() << global_filename + ".kat";
//            out << editor->toPlainText();
//        }

    } else {
        QString savefilename = QFileDialog::getSaveFileName(this,
            tr("Save File"),
            "",
            tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
        pDir = new QDir(".");

        fileDir = pDir->filePath(savefilename);
        tmpfileDir = pDir->filePath(tmp_filename);

        tmp_filename = savefilename + ".kat";

        get_file_dir();

        QFile file(savefilename);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << savefilename;
            out << editor->toPlainText();
        }

//        QFile file2(tmp_filename);
//        if (file2.open(QIODevice::ReadWrite))
//        {
//            QTextStream out(&file2);
//            qDebug() << tmp_filename;
//            out << editor->toPlainText();
//        }

        global_filename = savefilename;
        newfile = false;
    }
    update();
}

void MainWindow::save_as() {
    QString savefilename = QFileDialog::getSaveFileName(this,
        tr("Save File"),
        global_file_shortname,
        tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));

    pDir = new QDir(".");
    fileDir = pDir->filePath(savefilename);

    QFile file(savefilename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << savefilename;
        out << editor->toPlainText();
        global_filename = savefilename;
    }
    QTextStream out(&file);
    this->setWindowTitle(savefilename);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
    update();
}

void MainWindow::show_about() {
   QMessageBox Msgbox(QMessageBox::Information, "About", "   Kakapos intelligent IDE \n    0.5.0 under GNU GPL3\n Kvar_ispw17 Copyright 2017",QMessageBox::Ok ,this);
    Msgbox.setStandardButtons(QMessageBox::Ok);
    Msgbox.setButtonText(QMessageBox::Ok, QString("        OK        "));
    Msgbox.exec();
}


void MainWindow::build() {
    this->save_obj();
    QProcess p(0);
    QString path = QCoreApplication::applicationDirPath();
    QString arguments = " -Wall -m32 -o ";
    QString command = "g++ " + global_filename + arguments + g_filedir + global_filename.mid(0, global_filename.length() - 4);
    p.start(command);
    qDebug() << command;
    p.waitForStarted();
    p.waitForFinished();
}

void MainWindow::run(){
    QString command = global_filename.mid(0, global_filename.length() - 4)+ "& pause";
    QByteArray ba = command.toLatin1();
    char *mm = ba.data();
    std::system(mm);
}

void MainWindow::debug() {

}

void MainWindow::get_file_dir(){
    QRegExp match(global_filename);
    QRegExp disklabel(global_filename);
    disklabel.setPattern("[a-z_A-Z]:");
    match.setPattern("\/[a-z_A-Z_0-9]+");
    int pos = 0;
    QString str;
    std::vector<QString> vec;
    while ((pos = disklabel.indexIn(global_filename, pos)) != -1) {
        str = disklabel.cap(0);
        vec.push_back(str);
        break;
    }
    while ((pos = match.indexIn(global_filename, pos)) != -1) {
       str = match.cap(0);
       vec.push_back(str);
       pos += match.matchedLength();
    }
    global_file_shortname = str;
}

void MainWindow::start_astyle(){
    save_file(global_filename);
    qDebug() << "DO ASTYLE!";
    QProcess astyle;
    QString path;
    QDir dir;
    path = dir.absolutePath();
    astyle.start(path + "/release/astyle",QStringList() << "--style=kr" << "-p" << global_filename);
    qDebug() << path + "/release/astyle --style=kr -p " + global_filename;
    astyle.waitForFinished();
    while (astyle.canReadLine()) {
        qDebug() << astyle.readLine().trimmed();
    }
    editor->clear();
    pDir = new QDir(".");
    QFile file(global_filename);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
    update();
}
