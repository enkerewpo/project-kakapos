#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
from init_window import *

qtCreatorFile = "UI/init_window.ui";


def init():

    return;


if __name__ == '__main__':
    print("Starting manager...");
    app = QApplication(sys.argv);
    mainWindow = QMainWindow();
    ui = Ui_initwindow();
    ui.setupUi(mainWindow);
    mainWindow.show();
    sys.exit(app.exec_());
    init();
