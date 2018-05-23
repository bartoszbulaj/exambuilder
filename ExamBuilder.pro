#-------------------------------------------------
#
# Project created by QtCreator 2014-04-18T20:03:30
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExamBuilder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logo.cpp \
    form.cpp \
    testwithkey.cpp \
    testwithcheck.cpp \
    adminpanel.cpp

HEADERS  += mainwindow.h \
    logo.h \
    form.h \
    testwithkey.h \
    testwithcheck.h \
    adminpanel.h

FORMS    +=

RESOURCES += \
    Zasoby.qrc
