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
           src/data.h \
           src/datatypes.h \
           src/accountstab.h \
           src/accountsmodel.h \
           src/accounteditor.h \
           src/accountwidget.h \
           src/itemstab.h \
           src/itemsmodel.h \
           src/itemeditor.h \
           src/itemwidget.h \
           src/categoriestab.h \
           src/categoriesmodel.h \
           src/categoryeditor.h \
           src/categorywidget.h \
           src/entryeditor.h \
           src/lineedit.h \
           src/accountwidget.h \
           src/aboutbox.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/accountstab.cpp \
           src/accountsmodel.cpp \
           src/accounteditor.cpp \
           src/accountwidget.cpp \
           src/itemstab.cpp \
           src/itemsmodel.cpp \
           src/itemeditor.cpp \
           src/itemwidget.cpp \
           src/categoriestab.cpp \
           src/categoriesmodel.cpp \
           src/categoryeditor.cpp \
           src/categorywidget.cpp \
           src/entryeditor.cpp \
           src/lineedit.cpp \
           src/aboutbox.cpp
