TEMPLATE = app
TARGET = EZMarket
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
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
           src/imgtools.h \
           src/lineedit.h \
           src/spinbox.h \
           src/entryeditor.h \
           src/accountstab.h \
           src/accountsmodel.h \
           src/accounteditor.h \
           src/accountwidget.h \
           src/categoriestab.h \
           src/categoriesmodel.h \
           src/categoryeditor.h \
           src/categorywidget.h \
           src/itemstab.h \
           src/itemsmodel.h \
           src/itemeditor.h \
           src/itemwidget.h \
           src/soundboard.h \
           src/checkout.h \
           src/aboutbox.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/imgtools.cpp \
           src/lineedit.cpp \
           src/spinbox.cpp \
           src/entryeditor.cpp \
           src/accountstab.cpp \
           src/accountsmodel.cpp \
           src/accounteditor.cpp \
           src/accountwidget.cpp \
           src/categoriestab.cpp \
           src/categoriesmodel.cpp \
           src/categoryeditor.cpp \
           src/categorywidget.cpp \
           src/itemstab.cpp \
           src/itemsmodel.cpp \
           src/itemeditor.cpp \
           src/itemwidget.cpp \
           src/soundboard.cpp \
           src/checkout.cpp \
           src/aboutbox.cpp
