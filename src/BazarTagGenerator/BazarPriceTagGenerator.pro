#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T06:41:07
#
#-------------------------------------------------

QT       += core gui webkit xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = BazarPriceTagGenerator
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp\
        BarCodeGenerator.cpp \
    ConversionHelper.cpp

HEADERS  += MainWindow.h\
         BarCodeGenerator.h \
    ConversionHelper.h

FORMS    += MainWindow.ui

RESOURCES +=
