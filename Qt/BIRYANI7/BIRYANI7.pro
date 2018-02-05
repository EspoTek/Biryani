#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T13:35:23
#
#-------------------------------------------------

#Recommended for use with Qt 5.8 or similar
QT       += core gui
QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BIRYANI7
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
    inittransfer.cpp \
    usbinterface.cpp \
    datastreamer.cpp \
    qcustomplot.cpp \
    streamingdisplay.cpp \
    packetbuffer.cpp \
    o1buffer.cpp

HEADERS  += mainwindow.h\
        synamps_config.h \
    inittransfer.h \
    usbinterface.h \
    synamps_preinit.h \
    userdefinedproperties.h \
    datastreamer.h \
    qcustomplot.h \
    streamingdisplay.h \
    packetbuffer.h \
    o1buffer.h

DEFINES += "BIRYANI7_TARGET_DEVICE=SYNAMPS2"

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


FORMS    += mainwindow.ui \
    streamingdisplay.ui
