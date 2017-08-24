#ifndef HEADERS_H
#define HEADERS_H


/*
 * cp %{sourceDir}\plugin_framework\plugin_manager.py %{buildDir}\release\Scripts
 * cp %{sourceDir}\config\config.json %{buildDir}\release\Config
 *
 */


//python
#include <Python.h>

//cplusplus
#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/shadow_effect.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonValuePtr>
#include <QJsonValueRef>
#include <QJsonParseError>
#include <QJsonValueRefPtr>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <iostream>
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
#include <QTextStream>
#include "include/code_editor.h"

#endif // HEADERS_H
