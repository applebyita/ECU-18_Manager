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

#include "mapwidget.h"
#include "ui_mapwidget.h"
#include <QObject>
#include <QQuickItem>

#include <QQuickView>
#include <QWidget>
#include <QQmlEngine>

#include <QFile>

#include <QDebug>

#include "mainwindow.h"

MapWidget::MapWidget(int Width, int Height, double Latitude, double Longitude, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWidget)
{
    Q_UNUSED(Width);
    Q_UNUSED(Height);
    ui->setupUi(this);

    Parente = parent;

//#if defined Q_OS_ANDROID
    ui->quickWidget_Mappa->setVisible(true);

//    //Setta la cartella di lavoro
//    qDebug() << QString("Crea Path file!!!!!!!!!!!!!!!!!!!")+"\r\n";

//    ProjectPath=((MainWindow*)parent)->GetProjectPath();

//    qDebug() << QString("Path file: ")+ ProjectPath.absolutePath() +"§§";

//    //Setta l'url del file locale da leggere
// //   UrlMap= QString("file://")+ ProjectPath.absolutePath()+QString("/Map.htm");
//    UrlMap= QString("file:/")+ ProjectPath.absolutePath()+QString("/Map.htm");
//    qDebug() << QString("UrlMap: ")+ UrlMap;

//#else
//    //spengo il quickwidget
//    ui->quickWidget_Mappa->setVisible(false);
//    //crea il contenitore per la mappa qml
//    QQuickView *view = new QQuickView();
//    QWidget* container = QWidget::createWindowContainer(view, this);
//    // container->setMinimumSize(200, 200);
//    // container->setMaximumSize(200, 200); // the displaying size doesn't change with or without these lines
//    // container->setFocusPolicy(Qt::TabFocus);
//    view->setSource(QUrl("qrc:///Resources/map.qml"));
//    this->layout()->addWidget(container);
//#endif

    //MAP QML
//    #if defined Q_OS_ANDROID
    ContextMappa=ui->quickWidget_Mappa->rootContext();
//    #else
//    ContextMappa=view->rootContext();
//    #endif

    ContextMappa->setContextProperty("mainWidget", this);
//    emit UrlChanged();

 //   #if defined Q_OS_ANDROID
    QQuickItem* QML_Mappa_Item = qobject_cast<QQuickItem *>(ui->quickWidget_Mappa->rootObject());
//    #else
//    QQuickItem* QML_Mappa_Item = qobject_cast<QQuickItem *>(view->rootObject());
//    #endif

    // connect our C++ signal to our QML slot
    // NOTE: if we want to pass an parameter to our QML slot, it has to be
    // a QVariant.
          QObject::connect(this, SIGNAL(ReloadMap()), QML_Mappa_Item, SLOT(reloadMap()));
          QObject::connect(this, SIGNAL(EraseMap()), QML_Mappa_Item, SLOT(eraseMap()));
          QObject::connect(this, SIGNAL(TurnOffMap()), QML_Mappa_Item, SLOT(turnOff()));
          QObject::connect(this, SIGNAL(TurnOnMap()), QML_Mappa_Item, SLOT(turnOn()));
          QObject::connect(this, SIGNAL(ReloadMapFromUrl()), QML_Mappa_Item, SLOT(reloadMapFromUrl()));

//#ifndef Q_OS_ANDROID
//          QObject::connect(this, SIGNAL(updateSize(QVariant, QVariant)), QML_Mappa_Item, SLOT(setSize(QVariant,QVariant)));
//          QObject::connect(QML_Mappa_Item, SIGNAL(destroyMap()), this, SLOT(Destroy()));
//          QObject::connect(view->engine(), &QQmlEngine::quit, this, &QWidget::close);
////        QObject::connect(view->engine(), &QQmlEngine::quit, ((MainWindow*)parent), &MainWindow::DestroyMap);
////        QObject::connect(QML_Mappa_Item, SIGNAL(destroyMap()), ((MainWindow*)parent), &MainWindow::DestroyMap);

//  //    CaricaMappa();

//          //ridimensiona il qml
//          emit updateSize(Width, Height);
//#endif
          emit UrlChanged();

              //Inserisci le coordinate nel file html
                  FileHTML.clear();
                  FileHTML=FileHTMLModel;
                  QString Coordinate=tr("(%1,%2)").arg(Latitude).arg(Longitude);
 //                 QString Coordinate=tr("(%1,%2)").arg((double)(ListaECU.at(IndiceMacchinaAttiva))->Latitudine).arg((double)(ListaECU.at(IndiceMacchinaAttiva))->Longitudine);
                  FileHTML.replace("(44.9016223,9.851193599999988)",Coordinate);

//  #if defined Q_OS_ANDROID
//  //in android salva il file su disco per evitare l'errore html
//                  if (ProjectPath.exists()) {
//                         QString NomeFile=ProjectPath.absolutePath()+QString("/Map.htm");

//                         QFile file(NomeFile);

//                             if (QFile::exists(NomeFile))
//                             {

//                                 file.setPermissions(QFile::ReadOther | QFile::WriteOther);
//                                 file.remove();
//                             }


//                             if (file.open(QIODevice::ReadWrite)){
//                                 //Copia l'html modificato sul file
//                                 QTextStream DatOut(&file);
//                                 DatOut << FileHTML;
//                                 file.flush();
//                                 file.close();
//                             }

////                          // set the QML webview URL
////                          QQmlContext* Context=ui->quickWidget->rootContext();

////                          QVariant URLbase=QString("file:///sdcard/Documents/TEAM/Index.htm");
//                    }


//  #else
                  ContextMappa->setContextProperty("mainWidget", this);
                  emit FileHTMLChanged();
//  #endif


                  QApplication::processEvents();

 //                 qDebug()<<"Coordinate aggiornate:" << Coordinate;
          //    qDebug() << "MAP: lat:"<< (double)(ListaECU.at(IndiceMacchinaAttiva))->Latitudine <<" long: " << (double)(ListaECU.at(IndiceMacchinaAttiva))->Longitudine;

//#if defined Q_OS_ANDROID
//                  emit ReloadMapFromUrl();
//#else
                  emit ReloadMap();
//#endif
}

MapWidget::~MapWidget()
{
    delete ui;
}

void MapWidget::Show(void){

}

void MapWidget::Destroy(void){
//qDebug()<<"CHIUDO VIEW!!!!!!!!!!!!!!!!!!!!!!!!!";
//    view->deleteLater();
//    QApplication::processEvents();
//    qDebug()<<"CHIUDO CONTAINER!!!!!!!!!!!!!!!!!!!!!!!!!";
//    container->deleteLater();
//    QApplication::processEvents();
//    qDebug()<<"CHIUDO!!!!!!!!!!!!!!!!!!!!!!!!!";
//    close();
   ((MainWindow*) Parente)->DestroyMap();
}


