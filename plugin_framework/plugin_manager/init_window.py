# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'init_window.ui'
#
# Created by: PyQt5 UI code generator 5.9
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_initwindow(object):
    def setupUi(self, initwindow):
        initwindow.setObjectName("initwindow")
        initwindow.resize(592, 222)
        initwindow.setMinimumSize(QtCore.QSize(592, 222))
        initwindow.setMaximumSize(QtCore.QSize(592, 222))
        initwindow.setStyleSheet("background-color:rgb(50, 50, 50)")
        self.centralwidget = QtWidgets.QWidget(initwindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(170, 80, 241, 21))
        font = QtGui.QFont()
        font.setFamily("Segoe UI")
        font.setPointSize(12)
        font.setStyleStrategy(QtGui.QFont.PreferAntialias)
        self.label.setFont(font)
        self.label.setStyleSheet("color:white")
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        initwindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(initwindow)
        QtCore.QMetaObject.connectSlotsByName(initwindow)

    def retranslateUi(self, initwindow):
        _translate = QtCore.QCoreApplication.translate
        initwindow.setWindowTitle(_translate("initwindow", "Kakapos plugin manager"))
        self.label.setText(_translate("initwindow", "Scanning Plugins..."))

