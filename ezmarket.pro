TEMPLATE = app
TARGET = EZMarket
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug
RESOURCES += ezmarket.qrc
RC_FILE = ezmarket.rc
QT += widgets multimedia
TRANSLATIONS = ezmarket_da_DK.ts

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

# Input
HEADERS += src/mainwindow.h \
           src/datatypes.h \
           src/newentry.h \
           src/lineedit.h \
           src/accountwidget.h \
           src/itemwidget.h \
           src/aboutbox.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/newentry.cpp \
           src/lineedit.cpp \
           src/accountwidget.cpp \
           src/itemwidget.cpp \
           src/aboutbox.cpp
