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

#include <include/headers.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

CodeEditor *mainTextEditor;
QDir *fileLocationDir;
QString fileDir_str, fileDir_str_Tmp;
QString globalFilename_str, filename_str_Tmp, globalFileShortname_str, globalFileDir_str;
QString gcc_path;

bool isModified_b, isSaved_b;

bool newFile_b, isLoadsettings_b, isLoadplugins_b;
int dynamicWidth, dynamicHeight, userFontsize;

#ifdef Q_OS_WIN
int tabLength = 4;
#elif defined (Q_OS_OSX)
int tabLength = 4;
#else
int tabLength = 4;
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mainTextEditor = new CodeEditor(ui->centralWidget);
    loadSettings();
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::openObj);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveObj);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newObj);
    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::build);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::run);
    connect(ui->actionAStyle, &QAction::triggered, this, &MainWindow::startAstyle);
    connect(ui->actionKakapos_settings, &QAction::triggered, this, &MainWindow::loadSettings);
    connect(mainTextEditor, &CodeEditor::modified, this, &MainWindow::modified);
    connect(mainTextEditor, &CodeEditor::droppedFile, this, &MainWindow::dropOnEditor);

#ifdef Q_OS_WIN
    if (this->isMinimized()){
        this->showNormal();
    }
    this->activateWindow();
#endif
    versionTag = "0.0.2";
    setWindowTitle("kakapos " + versionTag);
    emit spreadVersionTag(versionTag);
    if(isLoadsettings_b) {
        ui->statuslabel->setText(QString("All settings have been loaded."));
    } else {
#ifdef Q_OS_WIN
        ui->statuslabel->setText(QString("Settings file not found. Please check your config/config.json"));
#elif defined(Q_OS_LINUX)
        ui->statuslabel->setText(QString("Settings file not found. Please check your ~/.kakapos_config.json"));
#endif
    }

    newFile_b = true;
    mainTextEditor->setWordWrapMode(QTextOption::NoWrap);
    mainTextEditor->setFocus();
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::dropOnEditor(QString dropFileName) {
    openObjFileWithFilename(dropFileName);
}

void MainWindow::update() {
    this->setWindowTitle(globalFilename_str);
    if(globalFilename_str.endsWith(".cpp", Qt::CaseInsensitive) ||
            globalFilename_str.endsWith(".cc", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".cxx", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".inc", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".c++", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".hpp", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".c", Qt::CaseInsensitive)  ||
            globalFilename_str.endsWith(".h", Qt::CaseInsensitive)) {
        highlighter = new HightlighterC(mainTextEditor->document());
        mainTextEditor->do_autotab = true;
        mainTextEditor->filetype = "cplusplus";
        ui->statuslabel->setText(QString("Parsing Syntax: C++"));
    }
    else {
        if(highlighter != NULL) delete highlighter;
        ui->statuslabel->setText(QString("Parsing Syntax: Plain text"));
    }
    this->setWindowTitle("kakapos " + versionTag + " - " +globalFilename_str);
}

void MainWindow::openObj() {
    if(isModified_b) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "New file - Warning", "File not saved. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
        if(rb == QMessageBox::Save)
        {
            saveObj();
        }
        if(rb == QMessageBox::Cancel) {
            return;
        }
    }
    QString fileName = \
            QFileDialog::getOpenFileName(this,
                                         tr("Open File"),
                                         "",
                                         tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.cxx;*.inc);;C files(*.c;*.h);;Plain text(*.txt)"));
    fileLocationDir = new QDir(".");
    globalFilename_str = fileName;
    getFileDir();
    update();
    fileDir_str = fileLocationDir->filePath(fileName);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    this->setWindowTitle("kakapos " + versionTag + " - " +globalFilename_str);
    mainTextEditor->clear();
    while(!file.atEnd()) {
        mainTextEditor->insertPlainText(out.readAll());
    }
}

void MainWindow::openObjFileWithFilename(QString fileName) {
    fileLocationDir = new QDir(".");
    globalFilename_str = fileName;
    fileDir_str = fileLocationDir->filePath(fileName);
    getFileDir();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        mainTextEditor->insertPlainText(out.readAll());
    }
    update();

}
void MainWindow::resizeEvent(QResizeEvent *event) {
    dynamicHeight = this->height();
    dynamicWidth = this->width();
    mainTextEditor->setGeometry(-1, 0, width() + 2, height() - 48);
    ui->statuslabel->setGeometry(14, this->height() - 47, this->width() - 2, 20);
}

void MainWindow::newObj() {
    globalFilename_str = "";
    newFile_b = true;
    if(isModified_b) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "New file - Warning", "File not saved. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
        if(rb == QMessageBox::Save)
        {
            saveObj();
        }
        if(rb == QMessageBox::Cancel) {
            return;
        }
    }
    mainTextEditor->clear();
    update();
}

void MainWindow::saveFileWithFilename(QString fileName) {
    fileLocationDir = new QDir(".");
    fileDir_str = fileLocationDir->filePath(fileName);
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << fileName;
        out << mainTextEditor->toPlainText();
    }
    update();
}

void MainWindow::saveObj() {
    if(!newFile_b) {
        fileLocationDir = new QDir(".");
        fileDir_str = fileLocationDir->filePath(globalFilename_str);
        QFile file(globalFilename_str);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << globalFilename_str;
            out << mainTextEditor->toPlainText();
        }

    } else {
        QString saveFilename_str = QFileDialog::getSaveFileName(this,
                                                            tr("Save File"),
                                                            "",
                                                            tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.cxx;*.inc);;C files(*.c;*.h);;Plain text(*.txt)"));
        fileLocationDir = new QDir(".");
        fileDir_str = fileLocationDir->filePath(saveFilename_str);
        getFileDir();
        QFile file(saveFilename_str);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << saveFilename_str;
            out << mainTextEditor->toPlainText();
        }
        globalFilename_str = saveFilename_str;
        newFile_b = false;
    }
    update();
}

void MainWindow::saveAs() {
    QString saveFilename_str = QFileDialog::getSaveFileName(this,
                                                        tr("Save File"),
                                                        globalFileShortname_str,
                                                        tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.cxx;*.inc);;C files(*.c;*.h);;Plain text(*.txt)"));
    fileLocationDir = new QDir(".");
    fileDir_str = fileLocationDir->filePath(saveFilename_str);

    QFile file(saveFilename_str);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << saveFilename_str;
        out << mainTextEditor->toPlainText();
        globalFilename_str = saveFilename_str;
    }
    QTextStream out(&file);
    this->setWindowTitle(saveFilename_str);
    while(!file.atEnd()) {
        mainTextEditor->insertPlainText(out.readAll());
    }
    update();
}

void MainWindow::showAbout() {
    AboutWindow * aboutWindow = new AboutWindow;
    aboutWindow->show();
}

void MainWindow::showLicense() {
    return;
}

void MainWindow::build() {
    this->saveObj();
    QProcess p(0);
    QString path = QCoreApplication::applicationDirPath();
    QString arguments = " -Wall -o ";
    QString command = "g++ " + globalFilename_str + arguments + globalFileDir_str + globalFilename_str.mid(0, globalFilename_str.length() - 4);
    p.start(command);
    qDebug() << command;
    p.waitForStarted();
    p.waitForFinished();
}

void MainWindow::run(){
    QString command = globalFilename_str.mid(0, globalFilename_str.length() - 4);
    QByteArray ba = command.toLatin1();
    char *mm = ba.data();
    std::system(mm);
}

void MainWindow::debug() {
    return;
}

void MainWindow::getFileDir(){
    QRegExp match(globalFilename_str);
    QRegExp diskLabel(globalFilename_str);
    diskLabel.setPattern("[a-z_A-Z]:");
    match.setPattern("\/[a-z_A-Z_0-9]+");
    int pos = 0;
    QString str;
    std::vector<QString> vec;
    while ((pos = diskLabel.indexIn(globalFilename_str, pos)) != -1) {
        str = diskLabel.cap(0);
        vec.push_back(str);
        break;
    }
    while ((pos = match.indexIn(globalFilename_str, pos)) != -1) {
        str = match.cap(0);
        vec.push_back(str);
        pos += match.matchedLength();
    }
    globalFileShortname_str = str;
}

void MainWindow::startAstyle(){
#ifdef ASTYLE_SUPPORT
    saveFile(globalFilename_str);
    qDebug() << "DO ASTYLE!";
    QProcess astyle;
    QString path;
    QDir dir;
    path = dir.absolutePath();
    astyle.start(path + "/astyle",QStringList() << "--style=kr" << "-p" << globalFilename_str);
    qDebug() << path + "/astyle --style=kr -p " + globalFilename_str;
    astyle.waitForFinished();
    while (astyle.canReadLine()) {
        qDebug() << astyle.readLine().trimmed();
    }
    mainTextEditor->clear();
    fileLocationDir = new QDir(".");
    QFile file(globalFilename_str);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        mainTextEditor->insertPlainText(out.readAll());
    }
    update();
#endif
}

void MainWindow::loadSettings() {
    QString val;
    QDir dir;
    QString path = dir.absolutePath();
    QFile JSON_Config_File;
    QString filename = path + "/config/config.json";
    JSON_Config_File.setFileName(filename);
    if(!JSON_Config_File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        isLoadsettings_b = false;
        QFont fdefault;
#ifdef Q_OS_WIN
        fdefault.setFamily("consolas");
#elif defined (Q_OS_OSX)
        fdefault.setFamily("Dejavu Sans Mono");
#else
        fdefault.setFamily("Monospace");
#endif
        fdefault.setPointSize(10);
        mainTextEditor->setFont(fdefault);
        QFontMetrics metrics(fdefault);
        mainTextEditor->setTabStopWidth(tabLength * metrics.width(' '));
        return;
    }
    isLoadsettings_b = true;
    val = JSON_Config_File.readAll();
    qDebug() << val;
    JSON_Config_File.close();
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(val.toUtf8(), &json_error);
    QFont &font = mainTextEditor->font;
    if(json_error.error == QJsonParseError::NoError)
    {
        QVariantMap res = parse_doucment.toVariant().toMap();
        font.setFamily(res["font-family"].toString());
        font.setPointSize(res["font-size"].toInt());
        QString style = res["font-style"].toString();
        if(style == "monospace") {
            font.setStyleHint(QFont::Monospace);
        }
        if(res["font-fixed-pitch"].toBool()) font.setFixedPitch(true);
    }
    mainTextEditor->setFont(font);
    QFontMetrics metrics(font);
    mainTextEditor->setTabStopWidth(tabLength * metrics.width(' '));
}

void MainWindow::loadPlugin() {
#ifdef PLUGIN_SUPPORT
    qDebug() << "IN LOADING";
    Py_Initialize();
    if(!Py_IsInitialized()) {
        qDebug() << "INITAL PYTHON ERROR";
        return;
    }
    PyObject *pModule = NULL;
    PyObject *pFunc   = NULL;
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("from PyQt5.QtWidgets import QApplication, QMainWindow");
    PyRun_SimpleString("from init_window import *");
    pModule = PyImport_ImportModule("plugin_manager");

    if(!pModule) {
        qDebug() << "LOAD MODULE ERROR";
        return;
    }

    pFunc   = PyObject_GetAttrString(pModule, "init");

    if(!pFunc) {
        qDebug() << "LOAD FUNCTION ERROR";
        return;
    }

    PyEval_CallObject(pFunc, NULL);
    Py_Finalize();
    qDebug() << "OUT LOADING";
    QProcess proc;
    proc.start("python plugin_manager.py");
    proc.waitForFinished();
    while (proc.canReadLine()) {
        qDebug() << proc.readLine().trimmed();
    }
#endif
}

void MainWindow::modified() {
    isModified_b = true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isModified_b) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "Quiting kakapos - Warning", "File not saved. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
        if(rb == QMessageBox::Save)
        {
            saveObj();
        }
        if(rb == QMessageBox::Cancel) {
            return;
        }
    }
}
