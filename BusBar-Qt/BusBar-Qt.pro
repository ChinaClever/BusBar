#-------------------------------------------------
#
# Project created by QtCreator 2017-07-15T09:35:41
#
#-------------------------------------------------

QT       += core gui
QT      += widgets printsupport
QT      += sql
QT      += network
QT      += androidextras
QT      += testlib
##QT += serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = busbar
TEMPLATE = app
CONFIG   += C++11



# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGSuntitled1

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include(test/test.pri)
include(common/common.pri)
include(modbus/modbus.pri)
include(sql/sql.pri)
include(dp/dp.pri)
include(home/home.pri)
include(line/line.pri)
include(branch/branch.pri)
include(logs/logs.pri)
include(setting/setting.pri)
include(services/services.pri)
include(shm/shm.pri)
include(net/net.pri)
include(thd/thd.pri)
#include(modbustcp/modbustcp.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    currentalarmsdlg.cpp \
    interfacechangesig.cpp \
    watchdogthread.cpp

HEADERS += \
        mainwindow.h \
    currentalarmsdlg.h \
    interfacechangesig.h \
    watchdogthread.h


FORMS += \
        mainwindow.ui \
    currentalarmsdlg.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/busbar/QtFullscreenActivity.java \
    android/src/an/qt/busbar/BroadcastReceiver.java

#contains(ANDROID_TARGET_ARCH,armeabi-v7a)
#{
#    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#}


#LIBS += -lts
#LIBS += -lpthread

target.path = /opt
INSTALLS += target

DISTFILES += \
    image/close.png










