QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lib
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

#应用图标
RC_ICONS += Lib.ico

#-------------------------------------
# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output


SOURCES += \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    MainWindow.h
