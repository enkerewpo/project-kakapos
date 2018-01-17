#include <include/headers.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

CodeEditor *editor;
QDir *fileLocationDir;
QString fileDir_String, fileDir_String_Temp;
QString globalFilename_String, filename_String_Temp, globalFileShortname_String, globalFileDir_String;
QString gcc_path;

bool isModified_Bool, isSaved_Bool;

bool newFile_Bool, isLoadsettings_Bool, isLoadplugins_Bool;
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
    editor = new CodeEditor(ui->centralWidget);
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
    connect(editor, &CodeEditor::modified, this, &MainWindow::modified);
#ifdef Q_OS_WIN
    if (this->isMinimized()){
        this->showNormal();
    }
    SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    this->activateWindow();
#endif
    tag = "0.5.9";
    setWindowTitle("kakapos " + tag);
    if(isLoadsettings_Bool) {
        ui->statuslabel->setText(QString("All settings have been loaded."));
    } else ui->statuslabel->setText(QString("Settings file not found. Please check your %INSTALLDIR%/JSON_Config_File/JSON_Config_File.json"));

    newFile_Bool = true;
    editor->setWordWrapMode(QTextOption::NoWrap);
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update() {
    this->setWindowTitle(globalFilename_String);
    if(globalFilename_String.endsWith(".cpp", Qt::CaseInsensitive) ||
            globalFilename_String.endsWith(".cc", Qt::CaseInsensitive)  ||
            globalFilename_String.endsWith(".c++", Qt::CaseInsensitive)  ||
            globalFilename_String.endsWith(".hpp", Qt::CaseInsensitive)  ||
            globalFilename_String.endsWith(".c", Qt::CaseInsensitive)  ||
            globalFilename_String.endsWith(".h", Qt::CaseInsensitive)) {
        highlighter = new Hightlighter_C(editor->document());
        editor->do_autotab = true;
        editor->filetype = "cplusplus";
        ui->statuslabel->setText(QString("Parsing Syntax: C++"));
    }
    else {
        if(highlighter != NULL) delete highlighter;
        ui->statuslabel->setText(QString("Parsing Syntax: Plain text"));
    }
    this->setWindowTitle("kakapos " + tag + " - " +globalFilename_String);
}

void MainWindow::openObj() {
    if(isModified_Bool) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "New file - Warning", "File not isSaved_Bool. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
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
                                         tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
    fileLocationDir = new QDir(".");
    globalFilename_String = fileName;
    getFileDir();
    update();
    fileDir_String = fileLocationDir->filePath(fileName);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    this->setWindowTitle("kakapos " + tag + " - " +globalFilename_String);
    editor->clear();
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
}

void MainWindow::openObjFile(QString fileName) {
    fileLocationDir = new QDir(".");
    globalFilename_String = fileName;
    fileDir_String = fileLocationDir->filePath(fileName);
    getFileDir();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
    update();

}
void MainWindow::resizeEvent(QResizeEvent *event) {
    dynamicHeight = this->height();
    dynamicWidth = this->width();
    editor->setGeometry(-1, 0, width() + 2, height() - 48);
    ui->statuslabel->setGeometry(14, this->height() - 47, this->width() - 2, 20);
}

void MainWindow::newObj() {
    globalFilename_String = "";
    newFile_Bool = true;
    if(isModified_Bool) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "New file - Warning", "File not isSaved_Bool. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
        if(rb == QMessageBox::Save)
        {
            saveObj();
        }
        if(rb == QMessageBox::Cancel) {
            return;
        }
    }
    editor->clear();
    update();
}

void MainWindow::saveFile(QString filename) {
    fileLocationDir = new QDir(".");
    fileDir_String = fileLocationDir->filePath(filename);
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << filename;
        out << editor->toPlainText();
    }
    update();
}

void MainWindow::saveObj() {
    qDebug() << newFile_Bool;
    if(!newFile_Bool) {
        fileLocationDir = new QDir(".");
        fileDir_String = fileLocationDir->filePath(globalFilename_String);
        QFile file(globalFilename_String);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << globalFilename_String;
            out << editor->toPlainText();
        }

    } else {
        QString saveFilename_String = QFileDialog::getSaveFileName(this,
                                                            tr("Save File"),
                                                            "",
                                                            tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
        fileLocationDir = new QDir(".");
        fileDir_String = fileLocationDir->filePath(saveFilename_String);
        getFileDir();
        QFile file(saveFilename_String);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            qDebug() << saveFilename_String;
            out << editor->toPlainText();
        }
        globalFilename_String = saveFilename_String;
        newFile_Bool = false;
    }
    update();
}

void MainWindow::saveAs() {
    QString saveFilename_String = QFileDialog::getSaveFileName(this,
                                                        tr("Save File"),
                                                        globalFileShortname_String,
                                                        tr("All file(*.*);;C++ files(*.cc;*.cpp;*.c++;*.hpp;*.h);;C files(*.c;*.h);;Plain text(*.txt)"));
    fileLocationDir = new QDir(".");
    fileDir_String = fileLocationDir->filePath(saveFilename_String);

    QFile file(saveFilename_String);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream out(&file);
        qDebug() << saveFilename_String;
        out << editor->toPlainText();
        globalFilename_String = saveFilename_String;
    }
    QTextStream out(&file);
    this->setWindowTitle(saveFilename_String);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
    }
    update();
}

void MainWindow::showAbout() {
    AboutWindow * aboutWindow = new AboutWindow;
    aboutWindow->tag = this->tag;
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
    QString command = "g++ " + globalFilename_String + arguments + globalFileDir_String + globalFilename_String.mid(0, globalFilename_String.length() - 4);
    p.start(command);
    qDebug() << command;
    p.waitForStarted();
    p.waitForFinished();
}

void MainWindow::run(){
    QString command = globalFilename_String.mid(0, globalFilename_String.length() - 4);
    QByteArray ba = command.toLatin1();
    char *mm = ba.data();
    std::system(mm);
}

void MainWindow::debug() {
    return;
}

void MainWindow::getFileDir(){
    QRegExp match(globalFilename_String);
    QRegExp diskLabel(globalFilename_String);
    diskLabel.setPattern("[a-z_A-Z]:");
    match.setPattern("\/[a-z_A-Z_0-9]+");
    int pos = 0;
    QString str;
    std::vector<QString> vec;
    while ((pos = diskLabel.indexIn(globalFilename_String, pos)) != -1) {
        str = diskLabel.cap(0);
        vec.push_back(str);
        break;
    }
    while ((pos = match.indexIn(globalFilename_String, pos)) != -1) {
        str = match.cap(0);
        vec.push_back(str);
        pos += match.matchedLength();
    }
    globalFileShortname_String = str;
}

void MainWindow::startAstyle(){
#ifdef ASTYLE_SUPPORT
    saveFile(globalFilename_String);
    qDebug() << "DO ASTYLE!";
    QProcess astyle;
    QString path;
    QDir dir;
    path = dir.absolutePath();
    astyle.start(path + "/astyle",QStringList() << "--style=kr" << "-p" << globalFilename_String);
    qDebug() << path + "/astyle --style=kr -p " + globalFilename_String;
    astyle.waitForFinished();
    while (astyle.canReadLine()) {
        qDebug() << astyle.readLine().trimmed();
    }
    editor->clear();
    fileLocationDir = new QDir(".");
    QFile file(globalFilename_String);
    if(!file.open(QIODevice::ReadWrite)) return;
    QTextStream out(&file);
    while(!file.atEnd()) {
        editor->insertPlainText(out.readAll());
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
        isLoadsettings_Bool = false;
        QFont fdefault;
#ifdef Q_OS_WIN
        fdefault.setFamily("consolas");
#elif defined (Q_OS_OSX)
        fdefault.setFamily("Dejavu Sans Mono");
#else
        fdefault.setFamily("Monospace");
#endif
        fdefault.setPointSize(10);
        editor->setFont(fdefault);
        QFontMetrics metrics(fdefault);
        editor->setTabStopWidth(tabLength * metrics.width(' '));
        return;
    }
    isLoadsettings_Bool = true;
    val = JSON_Config_File.readAll();
    qDebug() << val;
    JSON_Config_File.close();
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(val.toUtf8(), &json_error);
    QFont &font = editor->font;
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
    editor->setFont(font);
    QFontMetrics metrics(font);
    editor->setTabStopWidth(tabLength * metrics.width(' '));
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
    isModified_Bool = true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isModified_Bool) {
        QMessageBox::StandardButton rb = QMessageBox::warning(NULL, "Quiting kakapos - Warning", "File not isSaved_Bool. Do you want to save now?", QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard, QMessageBox::Save);
        if(rb == QMessageBox::Save)
        {
            saveObj();
        }
        if(rb == QMessageBox::Cancel) {
            return;
        }
    }
}
