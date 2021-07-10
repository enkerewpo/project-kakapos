<div align="center"><img src="https://enkerewpo.top/pics/icon2.png"></div>

------


<p align="center">
  <a title="Build Status" href="https://travis-ci.org/enkerewpo/project-kakapos"><img src="https://travis-ci.org/enkerewpo/project-kakapos.svg?branch=master"></a>
  <a title="Build Status" href="https://ci.appveyor.com/project/enkerewpo/project-kakapos"><img src="https://ci.appveyor.com/api/projects/status/i6vs11mn63dyager?svg=true"></a>
  <a class="badge-align" href="https://www.codacy.com/app/enkerewpo/project-kakapos?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=enkerewpo/project-kakapos&amp;utm_campaign=Badge_Grade"><img src="https://api.codacy.com/project/badge/Grade/f959a36c0fb94ad3adf3bdbc758ab49f"/></a>
    <a title="Join the chat at https://gitter.im/kakapos/Lobby" href="https://gitter.im/project-kakapos/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge"><img src="https://badges.gitter.im/project-kakapos/Lobby.svg"></a>
</p>

__Kakapos__ is a next-generation open-source programming integrated development environment (IDE).

Project kakapos IDE is licensed under the GNU General Public License v3.0, And some part of the source codes may be used under the licenses of BSD, etc. This software is written in __C++__ with _Qt6.1_.

## Build kakapos from source code
The source files are under the Qt project building method.
### 1.Install Qt6
1. Download Qt6.1 from the [official website](https://www.qt.io/download/).
2. Install it with the Qt6 with __MinGW Qt core__ just within the installer package to build this project.

### 2.Setting up qmake and make
1. Set Environment varibles in directory `PATH/TO/YOUR/QT/5.x.x/mingw_xx/bin/` to enable `qmake`.
2. Set Environment varibles in directory `PATH/TO/GCC/bin/` which has `make` program in it.

### 3.Commandline
```bash
$ cd /path/to/project-kakapos
$ qmake -project
$ qmake kakapos.pro
$ make
```
### 4.Install Python3 for plugin support
__kakapos__ needs __Python__ to implement plugins for kakapos. The simplest editor is wrote in C++, and the other features will be add-ons wrote in Python3.

GOTO [here](https://www.python.org/downloads/) to download it and make sure you have your `$PYHONHOME` and `$PYHONPATH` set to Python3 correctly.

## Development notification launchpad
This project is totally not official and so its codes maybe somehow messy.

### 2017.8.22
I just gave kakapos some simple auto-tab function and AStyle extension support.
The program is coming to be like a editor now... but now an inteligent IDE that it will be in the far furture.
Bugs to be fixed : terrible file directory split... and first time to write codes for C++ desktop program, the codes are terrible too...

### 2021.5.15
Upgraded to Qt 6.1 and the code won't get working! I probably should rewrite the code and get a decent naming rule.


Copyright (C) 2021 [Kvar_ispw17](mailto:enkerewpo@gmail.com)