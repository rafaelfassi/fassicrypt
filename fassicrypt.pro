#-------------------------------------------------
#
# Project created by QtCreator 2014-03-16T02:28:13
#
#-------------------------------------------------

QT += core qml quick

TARGET = FassiCrypt
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           simplecrypt.cpp

HEADERS  += mainwindow.h \
            simplecrypt.h

CONFIG += mobility

OTHER_FILES += \
    android/AndroidManifest.xml

RESOURCES += \
    fassicrypt.qrc

android: ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
