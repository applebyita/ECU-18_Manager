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

#include "wifiwidget.h"
#include "ui_wifiwidget.h"
#include "ui_progECU.h"
#include <QDebug>
#include <QInputEvent>
#include <QtNetwork>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#endif

/**
  revisioni
  0.1   13-03-18    Prima versione con swipe mouse
  0.2   13-03-18    Esempio Java con Fibonacci
  0.3   21-03-18    Integrazione programma Paolo
TODO:

 * Costanti QT per i vari sistemi operativi:
    * Q_OS_ANDROID
    * Q_OS_CYGWIN
    * Q_OS_FREEBSD
    * Q_OS_IOS
    * Q_OS_LINUX
    * Q_OS_MAC
    * Q_OS_OSX
    * Q_OS_QNX
    * Q_OS_UNIX
    * Q_OS_WIN32
    * Q_OS_WIN64
    * Q_OS_WIN
    * Q_OS_WINCE
    * Q_OS_WINPHONE
    * Q_OS_WINRT

********************************************************
 Main
*******************************************************/

WiFiWidget::WiFiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WiFiWidget)
{
    ui->setupUi(this);

   //Swipe init
    SwipeData.XStart=0;
    SwipeData.XEnd=0;
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    setMouseTracking(true);

   //prova a connetterti
     ConnectECU();
}

WiFiWidget::~WiFiWidget()
{
    delete ui;
}

//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//Inizio gestione swipe
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
bool WiFiWidget::event(QEvent *event)
{
bool handleEvent = false;
QInputEvent* inPut = dynamic_cast<QInputEvent*>(event);
QMouseEvent* MouseEvent = dynamic_cast<QMouseEvent*>(event);
QTouchEvent* TouchEvent = dynamic_cast<QTouchEvent*>(event);

    if(inPut){
        switch (inPut ->type()){
            case QEvent::TouchBegin:
                qDebug()<<"received TouchEvent BEGIN";
                inPut->accept();
                handleEvent=true;
                break;
            case QEvent::TouchUpdate:
               // qDebug()<<"received TouchEvent UPDATE";
                inPut->accept();
                handleEvent=true;
                break;
            case QEvent::TouchEnd:
                qDebug()<<"received TouchEvent END"<< TouchEvent->touchPoints();
                inPut->accept();
                handleEvent=true;
                if ((TouchEvent->touchPoints().at(0).lastPos().rx()-TouchEvent->touchPoints().at(0).startPos().rx())>100){
                    //Pagina avanti
                    PaginaAvanti();
                }else if ((TouchEvent->touchPoints().at(0).startPos().rx()-TouchEvent->touchPoints().at(0).lastPos().rx())>100){
                    //Pagina indietro
                    PaginaIndietro();
                }
                break;

            case QEvent::MouseButtonPress:
                qDebug()<<"received MouseEvent PRESS:" << MouseEvent->x();
                SwipeData.XStart=MouseEvent->x();
                SwipeData.XEnd=SwipeData.XStart;
                handleEvent=true;
                break;
            case QEvent::MouseButtonRelease:
                qDebug()<<"received MouseEvent RELEASE:" << MouseEvent->x();
                SwipeData.XEnd=MouseEvent->x();

                if (SwipeData.XEnd-SwipeData.XStart>100){
                    //Pagina avanti
                    PaginaAvanti();
                }else if (SwipeData.XStart-SwipeData.XEnd>100){
                    //Pagina indietro
                    PaginaIndietro();
                }
                handleEvent=true;
                break;
            default:
                break;
        }
//        return handleEvent;
    }
    return QWidget::event(event);
}

void WiFiWidget::PaginaAvanti(void){
qDebug()<<"Pagina avanti";
if (ui->stackedWidget->currentIndex()<(ui->stackedWidget->count()-1)) ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
}

void WiFiWidget::PaginaIndietro(void){
qDebug()<<"Pagina indietro";
if (ui->stackedWidget->currentIndex()>0) ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//Fine gestione swipe
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

//void WiFiWidget::UpdateLog(QTextEdit* Edit){
////    Edit->append("--------------------------------");
////    Edit->append("NetworkInterfaces");
////    Edit->append("--------------------------------");
////    Edit->append("UP AND RUNNING:");
//    QNetworkInterface* network_interface = new QNetworkInterface();
//       foreach (QNetworkInterface interface, network_interface->allInterfaces()) {
////           if((interface.flags() & QNetworkInterface::IsUp) && (interface.flags() & QNetworkInterface::IsRunning))
////              Edit->append("\t" + interface.name() + " " + interface.humanReadableName() + " " + interface.hardwareAddress());
//       }

////    Edit->append("");
////    Edit->append("--------------------------------------------");
////    Edit->append("Network Configuration Manager");
////    Edit->append("--------------------------------------------");
////    Edit->append("ACTIVE:");
//    NetworkManager.allConfigurations().clear();
//    WiFisList.clear();
//    connect(&NetworkManager,SIGNAL(updateCompleted()),this,SLOT(UpdateList()));
//    NetworkManager.updateConfigurations();
//}

//void WiFiWidget::UpdateList(void){
//    netcfgList = NetworkManager.allConfigurations();
//    WiFisList.clear();
//        foreach (QNetworkConfiguration x, netcfgList)
//        {
//       if (x.bearerType() == QNetworkConfiguration::BearerWLAN)
//    //    if (1)
//            {
//    //          foreach (QString Name, WiFisList){
//    //              if (x.name() == Name){
//    //                  return;
//    //              }
//    //          }

//                if(x.name() == "")
//                    WiFisList << "Unknown(Other Network)";
//                else
//                    WiFisList << x.name();
//                qDebug() << x.name();
//            //    ui->textEdit->append(x.name());
//            }
//        }
//    // Update our model
//    WiFisModel.setStringList(WiFisList);

////    ui->textEdit->append(tr("Fibonacci di %1 è:%2").arg(20).arg(fibonacci(20)));
//}

//#ifdef Q_OS_ANDROID
//int WiFiWidget::fibonacci(int n)
//{
//    return QAndroidJniObject::callStaticMethod<jint>
//                        ("esempio/MyClass" // class name
//                        , "fibonacci" // method name
//                        , "(I)I" // signature
//                        , n);
//}
//#endif



//void WiFiWidget::on_stackedWidget_currentChanged(int arg1)
//{
//    if (ui->stackedWidget->currentWidget()->accessibleName()==tr("page_progECU")){
//      ui->page_progECU->Connect();
//    }
//}

void WiFiWidget::ConnectECU(void){
    //Seleziona il widget di wait
    SetStackedWidget(tr("page_Attendi"),ui->stackedWidget);
    QTimer::singleShot(3000, this, SLOT(TryConnect()));
}

bool WiFiWidget::TryConnect() {
    if (ui->page_progECU->Connect()){
    //Connesso: visualizza la schermata di comando
        SetStackedWidget(tr("page_progECU"), ui->stackedWidget);
    return true;
    }else{
    //Errore di connessione
        SetStackedWidget(tr("page_ErroreConnessione"), ui->stackedWidget);
    return false;
    }
return false;
}

void  WiFiWidget::SetStackedWidget(QString Name, QStackedWidget* StackedWidget){

    for (int i=0;i<StackedWidget->count() ; i++)
    {
    QWidget* Widget = StackedWidget->widget(i);
        if (Widget->accessibleName()==Name){
            StackedWidget->setCurrentWidget(Widget);
            break;
        }
    }
}

void WiFiWidget::on_Retry_pushButton_clicked()
{
    ConnectECU();
}
