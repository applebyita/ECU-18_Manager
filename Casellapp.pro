#-------------------------------------------------
#
# Project created by QtCreator 2018-09-06T18:50:47
#
#-------------------------------------------------

QT       += core gui network quickwidgets qml quick webview widgets quickcontrols2
android{
QT      += androidextras
}

TARGET = ECU-18_Manager
TEMPLATE = app

VERSION = 2.7.0    # major.minor.patch
BUILDID = 50


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        notificationclient.cpp \
        pannelloecu.cpp \
        progECU.cpp \
        widgetecu.cpp \
        wifiwidget.cpp \
    backbuttonhandler.cpp \
    mapwidget.cpp \
    iosappstate.cpp

HEADERS += \
        mainwindow.h \
    ecu.h \
    notificationclient.h \
    pannelloecu.h \
    progECU.h \
    widgetecu.h \
    wifiwidget.h \
    backbuttonhandler.h \
    mapwidget.h \
    iosappstate.h \
    QtAppDelegate-C-Interface.h

FORMS += \
        mainwindow.ui \
        wifiwidget.ui \
        widgetecu.ui \
        progECU.ui \
        pannelloecu.ui \
    mapwidget.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

android{
DISTFILES += \
    android/src/org/qtproject/ecu18_manager/notification/NotificationClient.java \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

ios{

QMAKE_TARGET_BUNDLE_PREFIX = it.appleby
QMAKE_BUNDLE = ECU18-Manager

plist.input = ios/Info.plist
plist.output = $$OUT_PWD/Info.plist
QMAKE_SUBSTITUTES += plist
QMAKE_INFO_PLIST = $$OUT_PWD/Info.plist

#QMAKE_INFO_PLIST = ios/Info.plist

# icone da mettere nella cartella ios
QMAKE_ASSET_CATALOGS = $$PWD/ios/Images.xcassets
QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"


LIBS += -framework Foundation -framework UserNotifications

#ios-components
HEADERS += $$PWD/QtIosNotifier.h \
            $$PWD/AppDelegate.h

OBJECTIVE_SOURCES += $$PWD/QtIosNotifier.mm \
                     $$PWD/AppDelegate.mm

}
