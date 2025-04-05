QT += core gui widgets

CONFIG += c++20
CONFIG += qmake_qt_console

TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gameboard.cpp \
    cell.cpp \
    settingsdialog.cpp

HEADERS += \
    mainwindow.h \
    gameboard.h \
    cell.h \
    settingsdialog.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += minesweeper_ru.ts \
                minesweeper_en.ts
