/*
Copyright (C) 2022 Paolo Gaboardi

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3.0 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include <QApplication>
#include <QtWebView/QtWebView>
#include <QScreen>
#include "backbuttonhandler.h"

#ifdef Q_OS_IOS
#include "QtAppDelegate-C-Interface.h"
#endif

static QString Organizzazione;
static QString Dominio;
static QString Applicazione;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
    QApplication a(argc, argv);

    QtWebView::initialize();

//    int SizeHeight=a.screens().at(0)->size().height();
//    if (SizeHeight>300)
//        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Organizzazione=QGuiApplication::organizationName();
    Dominio=QGuiApplication::organizationDomain();
    Applicazione=QGuiApplication::applicationName();

//  register the QML timers
//  qmlRegisterType<MainWindow>("MyApp",1,0,"mainwindow");

    BackButtonHandler BackButtonObj;
    MainWindow w;

    #ifdef Q_OS_IOS
    QtAppDelegateInitialize(&a,&w);
    #endif

    QObject::connect(&BackButtonObj,SIGNAL(backButtonPressed()),&w,SLOT(onBackPressed()));

    w.showMaximized();
 //   w.show();

    return a.exec();
}


