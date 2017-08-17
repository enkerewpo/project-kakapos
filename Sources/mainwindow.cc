#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/customshadoweffect.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QLayout>
#include <QDebug>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <QTextStream>
#include "Headers/codeeditor.h"

CodeEditor *code_edit;
QDir *pDir;
QString fileDir;
QString global_filename, global_file_shortname, g_filedir;
QString gcc_path;
bool modified, saved, newfile;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::open_obj);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::show_about);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save_obj);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::save_as);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::new_obj);
    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::build);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::run);
    code_edit = new CodeEditor(ui->centralWidget);
    code_edit->setGeometry(-1, 0, width() + 2, height() - 20);
    code_edit->setFont(QFont("Consolas",13));
    code_edit->setStyleSheet("color:rgb(235,235,235);");
    code_edit->setTabStopWidth(40);
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update() {
    if(global_filename.endsWith(".cpp", Qt::CaseInsensitive) ||
       global_filename.endsWith(".cc", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".c++", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".hpp", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".c", Qt::CaseInsensitive)  ||
       global_filename.endsWith(".h", Qt::CaseInsensitive)) {
        highlighter = new Highlighter(code_edit->document());
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
    code_edit->clear();
    while(!file.atEnd()) {
        code_edit->insertPlainText(out.readAll());
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
    this->setWindowTitle(global_filename);
    while(!file.atEnd()) {
        code_edit->insertPlainText(out.readAll());
    }
    update();

}
void MainWindow::resizeEvent(QResizeEvent *event) {
    ui->centralWidget->resize(frameGeometry().size());
}

void MainWindow::new_obj() {
    global_filename = "";
    newfile = true;
    if(modified) {

    }
    code_edit->clear();
    update();
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
            out << code_edit->toPlainText();
        }
    } else {
        QString savefilename = QFileDialog::getSaveFileName(this,
            tr("Save File"),
            "",
            tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
        pDir = new QDir(".");
        fileDir = pDir->filePath(savefilename);
        get_file_dir();
        QFile file(savefilename);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << savefilename;
            out << code_edit->toPlainText();
        }
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
        out << code_edit->toPlainText();
        global_filename = savefilename;
    }
    QTextStream out(&file);
    this->setWindowTitle(savefilename);
    while(!file.atEnd()) {
        code_edit->insertPlainText(out.readAll());
    }
    update();
}

void MainWindow::show_about() {
   QMessageBox Msgbox(QMessageBox::Information, "About", "   Kakapos intelligent IDE \n    1.0.0 under GNU GPL3\n Kvar_ispw17 Copyright 2017",QMessageBox::Ok ,this);
    Msgbox.setStandardButtons(QMessageBox::Ok);
    Msgbox.setButtonText(QMessageBox::Ok, QString("        OK        "));
    Msgbox.exec();
}


void MainWindow::build() {
    this->save_obj();
    QProcess p(0);
    QString path = QCoreApplication::applicationDirPath();
    QString arguments = " -Wall -o -m32 ";
    QString command = "g++ " + global_filename + arguments + g_filedir + global_file_shortname;
    p.start(command);
    qDebug() << command;
    p.waitForStarted();
    p.waitForFinished();
}

void MainWindow::run(){
    QString command = g_filedir + global_file_shortname + "& pause";
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
//    for(auto it = vec.begin(); it != vec.end() - 1; it++) {
//        g_filedir += *it;
//    }
    global_file_shortname = str;
}
