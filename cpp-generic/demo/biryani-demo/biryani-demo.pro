#-------------------------------------------------
#
# Project created by QtCreator 2018-01-30T11:28:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = biryani-demo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../src/configurationfilehandler.cpp \
    ../../src/apiinterface.cpp \
    ../../src/phasezerohandler.cpp \
    ../../src/usbinterface.cpp \
    ../../src/inittransfer.cpp \
    ../../src/phaseonehandler.cpp \
    ../../src/phasetwohandler.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    ../../src/configurationfilehandler.h \
    ../../src/apiinterface.h \
    ../../src/phasezerohandler.h \
    ../../src/api_defines.h \
    ../../src/usbinterface.h \
    ../../src/synamps_init.h \
    ../../src/inittransfer.h \
    ../../src/phaseonehandler.h \
    ../../src/phasetwohandler.h \
    ../../src/phasetwothreading.h \
    qcustomplot.h

INCLUDEPATH += ../../src

#Libusb for Windows.
win32{
    contains(QT_ARCH, i386) {
        message("Cannot build for Windows on x86 platforms!")
    } else {
        message("Building for Windows (x64)")
        INCLUDEPATH += $$PWD/lib/win64/libusb/include/libusb-1.0
        LIBS += -L$$PWD/lib/win64/libusb/dll -llibusb-1.0
    }
}

FORMS    += mainwindow.ui
