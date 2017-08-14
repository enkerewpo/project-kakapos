<div align=center><img src="https://enkerewpo.github.io/images/Kakapos.png"></div> 

[![Build Status](https://travis-ci.org/enkerewpo/project-kakapos.svg?branch=master)](https://travis-ci.org/enkerewpo/project-kakapos)
[![Build status](https://ci.appveyor.com/api/projects/status/i6vs11mn63dyager?svg=true)](https://ci.appveyor.com/project/enkerewpo/project-kakapos)
![](https://img.shields.io/badge/language-C%2B%2B-red.svg)
![](https://img.shields.io/badge/language-Clojure-orange.svg)
![](https://img.shields.io/badge/license-GPLv3-blue.svg)
[![Join the chat at https://gitter.im/kakapos/Lobby](https://badges.gitter.im/project-kakapos/Lobby.svg)](https://gitter.im/project-kakapos/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
## Notice
_Project kakapos IDE_ is licensed under `the GNU General Public License v3.0`, And some part of the source codes may be used under the licenses of BSD, etc. This software is written in C++ and CLojure. 

## Introduction
[project-kakapos](https://github.com/enkerewpo/kakapos) is a next-generation open-source programming integrated development environment (IDE).

## Build from source code
The source files are under the Qt project building method.
### Install Qt5
1. Download Qt5.9 from the [official website](https://www.qt.io/download/).
2. Install it with the Qt5.9 with __MinGW 5.3.0 Qt core__ just within the installer package to build this project.

### Setting up qmake and make
1. Set Environment varibles in directory `PATH/TO/YOUR/QT/5.x.x/mingw_xx/bin/` to enable `qmake`.
2. Set Environment varibles in directory `PATH/TO/GCC/bin/` which has `make` program in it.

### Commandline
```bash
$ (enter the project-kakapos source root dirctory)
$ qmake -project
$ qmake kakapos.pro
$ make
```

Copyright (C) 2017 [Kvar_ispw17](mailto:enkerewpo@gmail.com)
