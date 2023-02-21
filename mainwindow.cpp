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
//#ifdef Q_OS_ANDROID
//#include "ui_mainwindowAndroid.h"
//#elif defined Q_OS_WIN
#include "ui_mainwindow.h"
//#endif
#include <QtNetwork>
#include <QScreen>
#include <QSize>
#include <QWindowStateChangeEvent>
#include <QMessageBox>
#include <QDebug>
#include "widgetecu.h"
#include <qscroller.h>
#include <QCryptographicHash>
#include <QSettings>
#include <QFile>
#include <QQuickItem>
#if defined Q_OS_IOS
#include "QtIosNotifier.h"
#endif




#define SOFTWARE_VERSION "rev. 2.07"

/**
  revisioni
  0.1   15-02-18    Prima versione con base http e notifica android
  0.2   20-02-18    Inizio separazione grafica windows/android/iphone
  0.3   01-03-18    Inizio disegno app
  0.4   07-03-18    Continuazione disegno app
  0.6   13-03-18    Creazione widget di selezione WiFi
  0.7   21-03-18    Integrazione WiFi Widget
  0.9   28-03-18    Aggiornamento progecu
  0.10  06-04-18    Attivazione lista widgets
  0.11  12-04-18    Attivazione visualizzazione stato remoto
  0.12  18-04-18    Attivazione colloquio con Dosso
  0.13  18-04-18    Attivazione mappa Google
  0.14  26-04-18    Fix mappa qml sempre sovrapposta
  0.15  4-05-18     Attivazione gestione lista macchine
  0.16  9-5-18      Completamento lista macchine
  0.17  16-5-18     Attivazione login
  0.18  21-5-18     Inizio comandi stop e timer
  0.19  30-5-18     Completamento comando stop
  0.20  6-6-18      Completamento comando timer
  0.22  22-6-18     Pubblicazione funzionante su Google Play
  0.23  29-6-18     Attivazione notifiche
  0.24  30-6-18     Attivazione move task to back in Android
  0.25  20-7-18     - Completamento move task to back in Android
                    - Fix cartella credenziali per tutti gli Android
                    - Fix schermata bianca al riavvio
                    - Fix richiesta comandi via internet
                    - Al riavvio va sempre sulla lista delle macchine
  0.26 27-7-18      - Attivazione notifiche Oreo
                    - Attivata la richiesta stato comando in modo da poter gestire i comandi multipli
  0.27 27-7-18      - Fix crash con Android 4.4
  0.28 3-8-18       - Fix crash sul cambio di utente
  0.29 30-8-18      - Fix notifiche che non si aggiornano
  0.31 12-9-18      - Riattivazione debug, sistemazione tasti timer, menu timer in qml
  0.32 21-9-18      - Attivazione mappa autoadattante
  0.33 24-9-18      - Versione per Google Play
  0.34 26-9-18      - Attivazione DEFINITIVA mappe
  0.35 5-10-18      - Fix mappe Android che non aggiornano per android 8
  0.36 12-10-18     - Attivazione Startec
  0.37 18-10-18     - Completamento Startec
  1.00 24-10-18     - Fix valori non validi negli stati startec
  1.01 16-11-18     - Fix contesto della mappa (non va una mazza)
  1.02 23-11-18     - Rifix tasto back su mappa, fix caricamento mappa
  1.03 23-11-18     - Centrata la sequenza per aggiornamere le mappe
  1.04 30-11-18     - Attivazione definitiva widget mappa con creazione/distruzione;
  1.05 6-12-18      - Fix font elenco;
  1.06 31-1-19      - Prima versione ios
  1.07 31-1-19      - Attivazione mappa ios
  1.08 14-2-19      - Mappa Android con salvataggio file html su disco
  1.09 14-2-19      - Mappa per ios
  2.00 14-3-19      - Prima versione per ios, nuovo elenco macchine
  2.00a 21-3-19     - Fix notifica android che si è persa
  2.00b 21-3-19     - Fix font pannello macchina, attivazione background ios
  2.01 5-4-19       - Fix macchina che non si seleziona
  2.03 9-4-19       - Attivazione nuove notifiche Ios
  2.04 18-4-19      - Attivazione schermata di fault sulla comunicazione Startec
  2.05 3-5-19       - Ripulitura finale prima di pubblicare su ios
  2.06 3-5-19       - Completamento app in background
  2.07 13-5-19      - Separazione routine di lettura dati per il background

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(qApp,  SIGNAL(applicationStateChanged(Qt::ApplicationState)),this, SLOT(stateChanged(Qt::ApplicationState)));

    ui->setupUi(this);


//    installEventFilter(this);

    hHTTP.ComandoAttuale=NESSUN_COMANDO;
    hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;

//    ui->lineEdit_Timer_Ore->installEventFilter(this);
//    ui->lineEdit_Timer_Minuti->installEventFilter(this);

//    ui->lineEdit_Timer_Ore->setVisible(false);
//    ui->lineEdit_Timer_Minuti->setVisible(false);
//    ui->label_TimerOre->setVisible(false);
//    ui->label_TimerMinuti->setVisible(false);

 //   TimerOre=0;
 //   TimerMinuti=0;

    //TUMBLER TIMER QML
    //Passa il mainwindow al qml del tumbler
    ContextTimer=ui->quickWidget_Timer->rootContext();
    ContextTimer->setContextProperty("MainWindow", this);
    QQuickItem* QML_Timer_Item = qobject_cast<QQuickItem *>(ui->quickWidget_Timer->rootObject());
     // connect our C++ signal to our QML slot
     // NOTE: if we want to pass an parameter to our QML slot, it has to be
     // a QVariant.
     QObject::connect(this, SIGNAL(DisableTimerQML()), QML_Timer_Item, SLOT(disableItem()));
     QObject::connect(this, SIGNAL(EnableTimerQML()), QML_Timer_Item, SLOT(enableItem()));



//     //MAP QML
//      ContextMappa=ui->quickWidget_Mappa->rootContext();
//      ContextMappa->setContextProperty("mainWindow", this);
//      QQuickItem* QML_Mappa_Item = qobject_cast<QQuickItem *>(ui->quickWidget_Mappa->rootObject());
//     // connect our C++ signal to our QML slot
//      // NOTE: if we want to pass an parameter to our QML slot, it has to be
//      // a QVariant.
//      QObject::connect(this, SIGNAL(ReloadMap()), QML_Mappa_Item, SLOT(reloadMap()));
//      QObject::connect(this, SIGNAL(EraseMap()), QML_Mappa_Item, SLOT(eraseMap()));
//      QObject::connect(this, SIGNAL(TurnOffMap()), QML_Mappa_Item, SLOT(turnOff()));
//      QObject::connect(this, SIGNAL(TurnOnMap()), QML_Mappa_Item, SLOT(turnOn()));

//      CaricaMappa();

      //TUMBLER VELOCITA'IMPOSTATA STARTEC QML
      Context_Vel_Imp_startec=ui->quickWidget_vel_imp_startec->rootContext();
      QQuickItem* QML_VelImpStartec_Item = qobject_cast<QQuickItem *>(ui->quickWidget_vel_imp_startec->rootObject());
       // connect our C++ signal to our QML slot
       // NOTE: if we want to pass an parameter to our QML slot, it has to be a QVariant.
//       QObject::connect(this, SIGNAL(DisableTimerQML()), QML_Timer_Item, SLOT(disableItem()));
//       QObject::connect(this, SIGNAL(EnableTimerQML()), QML_Timer_Item, SLOT(enableItem()));
      QObject::connect(QML_VelImpStartec_Item, SIGNAL(cifra_changed(int, int)), this, SLOT(VelImpStartec_Item_cifra_changed(int, int)));
      QObject::connect(this, SIGNAL(updateTumblerVel_Imp_startec(QVariant, QVariant, QVariant)), QML_VelImpStartec_Item, SLOT(updateTumbler(QVariant,QVariant,QVariant)));
      QObject::connect(this, SIGNAL(updateTumblerVel_Imp_startec_bianco()),QML_VelImpStartec_Item, SLOT(sfondoBianco()));
      QObject::connect(this, SIGNAL(updateTumblerVel_Imp_startec_giallo()),QML_VelImpStartec_Item, SLOT(sfondoGiallo()));


      //TUMBLER VELOCITA'IMPOSTATA STARTEC QML
      Context_Pausa_Start_startec=ui->quickWidget_pausa_start_startec->rootContext();
      QQuickItem* QML_PausaStartStartec_Item = qobject_cast<QQuickItem *>(ui->quickWidget_pausa_start_startec->rootObject());
       // connect our C++ signal to our QML slot
       // NOTE: if we want to pass an parameter to our QML slot, it has to be a QVariant.
//       QObject::connect(this, SIGNAL(DisableTimerQML()), QML_Timer_Item, SLOT(disableItem()));
//       QObject::connect(this, SIGNAL(EnableTimerQML()), QML_Timer_Item, SLOT(enableItem()));
      QObject::connect(QML_PausaStartStartec_Item, SIGNAL(cifra_changed(int, int)), this, SLOT(PausaStartStartec_Item_cifra_changed(int, int)));
      QObject::connect(this, SIGNAL(updateTumblerPausa_Start_startec(QVariant, QVariant, QVariant)), QML_PausaStartStartec_Item, SLOT(updateTumbler(QVariant,QVariant,QVariant)));
      QObject::connect(this, SIGNAL(updateTumblerPausa_Start_startec_bianco()),QML_PausaStartStartec_Item, SLOT(sfondoBianco()));
      QObject::connect(this, SIGNAL(updateTumblerPausa_Start_startec_giallo()),QML_PausaStartStartec_Item, SLOT(sfondoGiallo()));

      //TUMBLER VELOCITA'IMPOSTATA STARTEC QML
      Context_Pausa_End_startec=ui->quickWidget_pausa_end_startec->rootContext();
      QQuickItem* QML_Pausa_End_startec_Item = qobject_cast<QQuickItem *>(ui->quickWidget_pausa_end_startec->rootObject());
       // connect our C++ signal to our QML slot
       // NOTE: if we want to pass an parameter to our QML slot, it has to be a QVariant.
//       QObject::connect(this, SIGNAL(DisableTimerQML()), QML_Timer_Item, SLOT(disableItem()));
//       QObject::connect(this, SIGNAL(EnableTimerQML()), QML_Timer_Item, SLOT(enableItem()));
      QObject::connect(QML_Pausa_End_startec_Item, SIGNAL(cifra_changed(int, int)), this, SLOT(PausaEndStartec_Item_cifra_changed(int, int)));
      QObject::connect(this, SIGNAL(updateTumblerPausa_End_startec(QVariant, QVariant, QVariant)), QML_Pausa_End_startec_Item, SLOT(updateTumbler(QVariant,QVariant,QVariant)));
      QObject::connect(this, SIGNAL(updateTumblerPausa_End_startec_bianco()),QML_Pausa_End_startec_Item, SLOT(sfondoBianco()));
      QObject::connect(this, SIGNAL(updateTumblerPausa_End_startec_giallo()),QML_Pausa_End_startec_Item, SLOT(sfondoGiallo()));

     //LA PEZZA!!! Manda i tumbler a 111 sennò non si vedono le cifre
     Vel_Imp_startec=111;
     Pausa_Start_startec=111;
     Pausa_End_startec=111;
     emit updateTumblerVel_Imp_startec(Vel_Imp_startec/100,(Vel_Imp_startec%100)/10,(Vel_Imp_startec%100)%10);
     emit updateTumblerPausa_Start_startec(Pausa_Start_startec/100,((Pausa_Start_startec%100)/10),((Pausa_Start_startec%100)%10));
     emit updateTumblerPausa_End_startec(Pausa_End_startec/100,(Pausa_End_startec%100)/10,(Pausa_End_startec%100)%10);


     qDebug()<<"ATTENTION: Device Pixel Ratio="<<qApp->devicePixelRatio();
     qDebug()<<"ATTENTION: DPI="<<qApp->screens().at(0)->logicalDotsPerInch();
     qDebug()<<"ATTENTION: DPI X="<<qApp->screens().at(0)->logicalDotsPerInchX();
     qDebug()<<"ATTENTION: DPI Y="<<qApp->screens().at(0)->logicalDotsPerInchY();
     qDebug()<<"ATTENTION: Screen width="<<qApp->screens().at(0)->geometry().width();
     qDebug()<<"ATTENTION: Screen height="<<qApp->screens().at(0)->geometry().height();

    int width= ui->lineEdit_Username->width();
    ui->lineEdit_Username->setGeometry(0,0,width,static_cast<int>(qApp->fontMetrics().height()*1.2));
    ui->lineEdit_Password->setGeometry(0,0,width,static_cast<int>(qApp->fontMetrics().height()*1.2));
    width= ui->pushButton_Login->width();
    ui->pushButton_Login->setGeometry(0,0,width,qApp->fontMetrics().height()*3);

    ui->stackedWidget_App->setCurrentIndex(0);
    ui->pushButton_Connect->setVisible(true);
 //   ui->pushButton_Startec->setEnabled(false);
//    ui->pushButton_Mappa->setEnabled(false);
    ui->label_2->setText(tr("ECU18-APP ")+tr(SOFTWARE_VERSION));

#ifdef Q_OS_ANDROID
    notificationClient = new NotificationClient(this);
//    ui->horizontalSpacer->changeSize(0,0);
#else
    ui->label_Port->setVisible(false);
    ui->label_IP->setVisible(false);
    ui->lineEdit_IP->setVisible(false);
    ui->lineEdit_Port->setVisible(false);
#endif
    ContatoreResize=0;

#ifdef Q_OS_ANDROID
    //definizione scroller
    QScroller* scroller=QScroller::scroller(ui->listWidget_ECU);
    scroller->grabGesture(ui->listWidget_ECU,QScroller::LeftMouseButtonGesture);
    QScrollerProperties ActualProperties;
    ActualProperties=scroller->scrollerProperties();
    QVariant VerticalOvershoot=QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    ActualProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, VerticalOvershoot);
    scroller->setScrollerProperties(ActualProperties);
#endif

    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Elenco));
    hHTTP.ComandoAttuale=NESSUN_COMANDO;

    ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Core));

   // ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_3));

    //Check dello username
    //Setta la cartella di lavoro
    QString   WorkingDirectory;
    //WorkingDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/TEAM"));
    //WorkingDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QString("/TEAM"));
    WorkingDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/TEAM"));

    qDebug() << "Cartella settings:" << WorkingDirectory;

    ProjectPath.setPath(WorkingDirectory);

    if (!ProjectPath.exists()) {
 //       ProjectPath.mkdir(WorkingDirectory);
        ProjectPath.mkpath(WorkingDirectory);
    }

    QFile FileIni(ProjectPath.absolutePath()+QString("/Settings.ini"));

    if (FileIni.exists()){
       QSettings settings (ProjectPath.absolutePath()+QString("/Settings.ini"), QSettings::IniFormat);
       settings.beginGroup("User_Settings");
       UserName=settings.value("UserName").toString();
       Password=settings.value("Password").toString();
       settings.endGroup();
       RichiediElencoMacchine();
    }else{
        //Se il file non esiste svuota i campi e attendi l'utente
        UserName="";
        Password="";
        ui->lineEdit_Username->setText(UserName);
        ui->lineEdit_Password->setText(Password);
        emit ui->pushButton_User->click();
    }

   TipoComandoAsync=NO_COMMAND;
   hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;

   //Configura e avvia la base tempi
   connect(&BaseTempi,SIGNAL(timeout()),this,SLOT(CallbackBaseTempi()));
   BaseTempi.setInterval(BASE_TEMPI);
   BaseTempi.start(); //avvia la base tempi a 1 s
   ui->pushButton_User->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{
//    ShowTerminalMessage(ui->textEdit,QString("Connecting to %1:%2 Server").arg(ui->lineEdit_IP->text()).arg(ui->lineEdit_Port->text().toInt()));
//    tcpClient.connectToHost(ui->lineEdit_IP->text(), ui->lineEdit_Port->text().toInt());

//    if(tcpClient.waitForConnected(3000)){
//        ShowTerminalAssertMessage(ui->textEdit,"Connected!");
//    }else{
//       ShowTerminalErrorMessage(ui->textEdit,"Not Connected!");
//    }

 qDebug() << "Notify!";
}

void MainWindow::ShowTerminalMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
//    Edit->setStyleSheet("QTextEdit {background-color: black;font: 12pt \"MS Shell Dlg 2\";}");
    Edit->setTextColor(Qt::white);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::ShowTerminalErrorMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
//    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::red);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::ShowTerminalAssertMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
//    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::green);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::IncomingData(void){
  // ShowTerminalMessage(ui->textEdit, QString::fromStdString(tcpClient.readAll().toStdString()));
}

//void MainWindow::on_pushButton_Send_clicked()
//{
//#ifdef Q_OS_ANDROID
//    qDebug() << "Send notification!";
//    if (NotificationID<1000) NotificationID++;
//    notificationClient->setNotification(tr("Test!"),NotificationID);
//#endif

//    const QString urlSpec = ui->lineEdit_Data->text().trimmed();
//    if (urlSpec.isEmpty())
//       return;

//////    const QUrl newUrl = QUrl::fromUserInput(urlSpec);

//////    // schedule the request
//////    startRequest(newUrl);
//}

void MainWindow::startRequest(const QUrl &requestedUrl)
{
    if (TimerTimeoutConnessione==0){
        url = requestedUrl;
        httpRequestAborted = false;

        if (!url.isValid()) {
            ShowTerminalErrorMessage(ui->textEdit,tr("Invalid URL: %1: %2").arg(url.toString(), url.errorString()));
            return;
        }

        ui->pushButton_Send->setEnabled(false);

        ShowTerminalMessage(ui->textEdit,tr("Request URL: %1").arg(url.toString()));

        reply = qnam.get(QNetworkRequest(url));
        connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
        connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
        TimerTimeoutConnessione=TIMEOUT_CONNESSIONE;
    }
}

void MainWindow::startRequest_Async(const QUrl &requestedUrl)
{
    urlAsync = requestedUrl;
    httpRequestAbortedAsync = false;

    if (!urlAsync.isValid()) {
        ShowTerminalErrorMessage(ui->textEdit,tr("Invalid URLAsync: %1: %2").arg(urlAsync.toString(), url.errorString()));
        return;
    }

    ui->pushButton_Send->setEnabled(false);

    ShowTerminalAssertMessage(ui->textEdit,tr("Request URLAsync: %1").arg(urlAsync.toString()));
//    ShowTerminalMessage(ui->textEdit,tr("Request URLAsync: %1").arg(urlAsync.toString()));

    replyAsync = qnam.get(QNetworkRequest(urlAsync));
    connect(replyAsync, SIGNAL(finished()), this, SLOT(httpFinishedAsync()));
    connect(replyAsync, SIGNAL(readyRead()), this, SLOT(httpReadyReadAsync()));
    TimerTimeoutConnessioneAsync=TIMEOUT_CONNESSIONE;
}

void MainWindow::httpFinished()
{
    TimerTimeoutConnessione=0;
    ui->pushButton_Send->setEnabled(true);

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = Q_NULLPTR;
        hHTTP.ComandoAttuale=NESSUN_COMANDO;
        hHTTP.Result=HTTP_ERROR;
        ui->stackedWidget_ElencoMacchine->setCurrentIndex(1);
        return;
    }

    if (reply->error()) {
        ShowTerminalErrorMessage(ui->textEdit,tr("URL ERROR: %1").arg(reply->errorString()));
        reply->deleteLater();
        reply = Q_NULLPTR;
        hHTTP.ComandoAttuale=NESSUN_COMANDO;
        hHTTP.Result=HTTP_ERROR;
        ui->stackedWidget_ElencoMacchine->setCurrentIndex(1);
        return;
    }


    reply->deleteLater();
    reply = Q_NULLPTR;

    if (hHTTP.Result==HTTP_ERROR){
        InviaComandoHTTP(hHTTP.ComandoAttuale,hHTTP.IndiceMacchinaComando);
    }else{
     ui->stackedWidget_ElencoMacchine->setCurrentIndex(ui->stackedWidget_ElencoMacchine->indexOf(ui->page_Elenco));
    //ho finito un comando valido, verifica se è singolo o se devo andae avanti con le richieste
        if ((hHTTP.ComandoAttuale==DOWNLOAD_DETTAGLI_MACCHINE)||(hHTTP.ComandoAttuale==DOWNLOAD_STATO_MACCHINA)){
            hHTTP.IndiceMacchinaComando++;
            qDebug() << "Indice Macchina:" << hHTTP.IndiceMacchinaComando;
            if (hHTTP.IndiceMacchinaComando<ListaECU.count()){
                Comandi_Server_t  ComandoTemp=hHTTP.ComandoAttuale;
                hHTTP.ComandoAttuale=NESSUN_COMANDO;
                InviaComandoHTTP(ComandoTemp,hHTTP.IndiceMacchinaComando);
            } else{
                if(hHTTP.ComandoAttuale==DOWNLOAD_DETTAGLI_MACCHINE){ 
                    Refresh_Lista_ECU();
                    ui->stackedWidget_ElencoMacchine->setCurrentIndex(ui->stackedWidget_ElencoMacchine->indexOf(ui->page_Elenco));
                  hHTTP.ComandoAttuale=NESSUN_COMANDO;
                  InviaComandoHTTP(DOWNLOAD_STATO_MACCHINA, 0);
                }
                else{
                  hHTTP.ComandoAttuale=NESSUN_COMANDO;
                  Refresh_Lista_ECU();
                }
            }
        }else if(hHTTP.ComandoAttuale==DOWNLOAD_ELENCO_MACCHINE){
            hHTTP.ComandoAttuale=NESSUN_COMANDO;
            //Elenco corretto
            if (ListaECU.count()){
            ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Core));
 //              ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_3));
            ui->pushButton_User->setChecked(false);
            InviaComandoHTTP(DOWNLOAD_DETTAGLI_MACCHINE, 0);
            }else{
            //Elenco non corretto
                if (!(ui->pushButton_User->isChecked()))  emit ui->pushButton_User->click();
               ui->label_errore_credenziali->setText(tr("Credenziali errate!"));
            }

        }else hHTTP.ComandoAttuale=NESSUN_COMANDO;
    }
}

void MainWindow::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    QString replyString=QString::fromStdString(reply->readAll().toStdString());

    ShowTerminalMessage(ui->textEdit,tr("answer: %1").arg(replyString));
    InterpreteRispostaHttp(replyString);
}

void MainWindow::httpFinishedAsync()
{
    TimerTimeoutConnessioneAsync=0;
    ui->pushButton_Send->setEnabled(true);

    if (httpRequestAbortedAsync) {
        replyAsync->deleteLater();
        replyAsync = Q_NULLPTR;
        hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
        hHTTPAsync.Result=HTTP_ERROR;
        return;
    }

    if (replyAsync->error()) {
        ShowTerminalErrorMessage(ui->textEdit,tr("URLAsync ERROR: %1").arg(replyAsync->errorString()));
        replyAsync->deleteLater();
        replyAsync = Q_NULLPTR;
        hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
        hHTTPAsync.Result=HTTP_ERROR;
        return;
    }

    replyAsync->deleteLater();
    replyAsync = Q_NULLPTR;

    if (hHTTPAsync.Result==HTTP_ERROR){
        InviaComandoHTTP_Async(hHTTPAsync.ComandoAttuale,hHTTPAsync.IndiceMacchinaComando);
    }else{
    //ho finito un comando valido, verifica se è singolo o se devo andare avanti con le richieste
       if (hHTTPAsync.ComandoAttuale==DOWNLOAD_COMANDO_MACCHINA){
            hHTTPAsync.IndiceMacchinaComando++;
            if (hHTTPAsync.IndiceMacchinaComando<ListaECU.count()){
                Comandi_Server_t  ComandoTemp=hHTTPAsync.ComandoAttuale;
                hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
                InviaComandoHTTP_Async(ComandoTemp,hHTTPAsync.IndiceMacchinaComando);
            }else{
                ComandiUpdated=true;
                hHTTPAsync.IndiceMacchinaComando=-1;
                hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
                UpdateTastiTimerStop();
            }
       }else{
           hHTTPAsync.IndiceMacchinaComando=-1;
           hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
       }

       CheckComandoEseguito=true;
    }
}

void MainWindow::httpReadyReadAsync()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    QString replyString=QString::fromStdString(replyAsync->readAll().toStdString());

    ShowTerminalAssertMessage(ui->textEdit,tr("Async answer: %1").arg(replyString));
//    ShowTerminalMessage(ui->textEdit,tr("Async answer: %1").arg(replyString));
    InterpreteRispostaHttpAsync(replyString);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    #ifdef Q_OS_ANDROID
        qDebug() << "Send notification!";
        if (NotificationID<1000) NotificationID++;
        notificationClient->setNotification(tr("Test!"),NotificationID);
    #elif defined Q_OS_IOS
        qDebug() << "Send notification!";
        QtIosNotifier* Notifier=new(QtIosNotifier);
        Notifier->show(tr("Ciao"),tr("Fabrizio"),10);
    #endif
}

void MainWindow::changeEvent( QEvent* e )
{
    if( e->type() == QEvent::WindowStateChange )
    {
        QWindowStateChangeEvent* event = static_cast< QWindowStateChangeEvent* >( e );

        if( event->oldState() & Qt::WindowMinimized )
        {
            qDebug() << "Window restored (to normal or maximized state)!";
        }
        else if( event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized )
        {
            qDebug() << "Window Maximized!";
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    #ifdef Q_OS_IOS
    if (1){
    #else
    if (ContatoreResize==1){
    #endif

    //ridimensiona tasto team
        QSize IconSizes;
        #ifdef Q_OS_IOS
        IconSizes.setHeight((int)(ui->pushButton_Team->height()*1.5));
        IconSizes.setWidth((int)(ui->pushButton_Team->width()*1.5));
        #else
        IconSizes.setHeight((int)(ui->pushButton_Team->height()*0.9));
        IconSizes.setWidth((int)(ui->pushButton_Team->width()*0.9));
        #endif
        ui->pushButton_Team->setIconSize(IconSizes);

    //ridimensiona tasto user
        #ifdef Q_OS_IOS
        IconSizes.setHeight((int)(ui->pushButton_User->height()*1.4));
        IconSizes.setWidth((int)(ui->pushButton_User->height()*1.4));
        #else
        IconSizes.setHeight((int)(ui->pushButton_User->height()*0.9));
        IconSizes.setWidth((int)(ui->pushButton_User->height()*0.9));
        #endif
        ui->pushButton_User->setGeometry(0,0,ui->pushButton_User->height(),ui->pushButton_User->height());
        ui->pushButton_User->setIconSize(IconSizes);

 #if defined Q_OS_ANDROID
        int Width;
        Width=ui->pushButton_Elenco->width();
        ui->pushButton_Elenco->setFixedHeight(Width);
        ui->pushButton_Elenco->setIconSize(QSize((int)(0.8*Width),(int)(0.8*Width)));
        ui->pushButton_Motore->setFixedHeight(Width);
        ui->pushButton_Motore->setIconSize(QSize((int)(0.8*Width),(int)(0.8*Width)));
        ui->pushButton_Startec->setFixedHeight(Width);
        ui->pushButton_Startec->setIconSize(QSize((int)(0.8*Width),(int)(0.8*Width)));
        ui->pushButton_Mappa->setFixedHeight(Width);
        ui->pushButton_Mappa->setIconSize(QSize((int)(0.8*Width),(int)(0.8*Width)));
        ui->pushButton_Wifi->setFixedHeight(Width);
        ui->pushButton_Wifi->setIconSize(QSize((int)(0.8*Width),(int)(0.8*Width)));
#elif defined Q_OS_IOS
        int Width;
        Width=ui->pushButton_Elenco->width()*0.8;
        ui->pushButton_Elenco->setFixedHeight(Width);
        ui->pushButton_Elenco->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Motore->setFixedHeight(Width);
        ui->pushButton_Motore->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Startec->setFixedHeight(Width);
        ui->pushButton_Startec->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Mappa->setFixedHeight(Width);
        ui->pushButton_Mappa->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Wifi->setFixedHeight(Width);
        ui->pushButton_Wifi->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
#endif

//        Refresh_Lista_ECU();

  }
if (ContatoreResize<10) ContatoreResize++;
}


void MainWindow::on_pushButton_Team_clicked()
{
 //    ui->stackedWidget->setCurrentIndex(0); //Commentare per disabilitare il debug
}

void MainWindow::on_pushButton_Elenco_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(0);
//    //emit TurnOffMap();
//    ui->stackedWidget_App->show();
//    ui->stackedWidget_App->activateWindow();
//    ui->stackedWidget_App->raise();
}

void MainWindow::on_pushButton_Motore_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(1);
    //emit TurnOffMap();

    TimerOre=0;
    emit TimerOreChanged();
    TimerMinuti=0;
    emit TimerMinutiChanged();

    if (IndiceMacchinaAttiva!=-1){
        ui->label_NomeMacchina->setText(ListaECU.at(IndiceMacchinaAttiva)->Nome);
        ui->label_UltimaLettura->setText(ListaECU.at(IndiceMacchinaAttiva)->UltimaLettura.toString("dd-MM-yyyy  hh:mm"));
        if ((!(ListaECU.at(IndiceMacchinaAttiva)->Status & MACHINE_ERROR))&&(!(ListaECU.at(IndiceMacchinaAttiva)->Status & MACHINE_WARNING))){
            ui->stackedWidget_Pannello->setCurrentIndex(0);
            ui->PannelloMotoreRemoto->setEnabled(true);
            ui->PannelloMotoreRemoto->SetGiriMotore(ListaECU.at(IndiceMacchinaAttiva)->GiriMotore);
            ui->PannelloMotoreRemoto->SetVbat(ListaECU.at(IndiceMacchinaAttiva)->VBat);
            ui->PannelloMotoreRemoto->SetGasolio(ListaECU.at(IndiceMacchinaAttiva)->Gasolio);
            ui->PannelloMotoreRemoto->SetPressioneDepressione(ListaECU.at(IndiceMacchinaAttiva)->Pressione,ListaECU.at(IndiceMacchinaAttiva)->Depressione);
            ui->PannelloMotoreRemoto->SetPressRecPressRecImp(ListaECU.at(IndiceMacchinaAttiva)->PressRec,ListaECU.at(IndiceMacchinaAttiva)->PressRecImp);
            ui->PannelloMotoreRemoto->SetTempH2O(ListaECU.at(IndiceMacchinaAttiva)->TempH2O);
            UpdateTastiTimerStop();
        }else{
            UpdateTastiTimerStop();

            //Update icona allarme/warning

            if (ListaECU.at(IndiceMacchinaAttiva)->Status & MACHINE_ERROR){
            //Warning
                ui->pushButton_Allarme->setIcon(QIcon(":/Resources/AllarmeTrasparente.png"));
            }else{
            //Allarme
                ui->pushButton_Allarme->setIcon(QIcon(":/Resources/WarningTrasparente.png"));
            }

            //ridimensiona icona
                QSize IconSizes;
                IconSizes.setHeight((int)((qApp->screens().at(0)->geometry().width()/100)*20));
                IconSizes.setWidth((int)((qApp->screens().at(0)->geometry().width()/100)*20));
                ui->pushButton_Allarme->setIconSize(IconSizes);


            ui->stackedWidget_Pannello->setCurrentIndex(1);


                switch(ListaECU.at(IndiceMacchinaAttiva)->StatoMotore){
                case ALLARME_RADIATORE:
                    ui->label_Nome_Allarme->setText(tr("Allarme radiatore!"));
                    break;
                case ALLARME_PRESSIONE:
                    ui->label_Nome_Allarme->setText(tr("Allarme pressione!"));
                    break;
                case ALLARME_ACQUA_MOTORE:
                    ui->label_Nome_Allarme->setText(tr("Allarme acqua motore!"));
                    break;
                case ALLARME_ALTERNATORE:
                    ui->label_Nome_Allarme->setText(tr("Allarme alternatore!"));
                    break;
                case ALLARME_OLIO:
                    ui->label_Nome_Allarme->setText(tr("Allarme olio!"));
                    break;
                case ALLARME_RISERVA:
                    ui->label_Nome_Allarme->setText(tr("Allarme riserva gasolio!"));
                    break;
                case FINE_LAVORO:
                    ui->label_Nome_Allarme->setText(tr("Fine lavoro"));
                    break;
                case ALLARME_FILTRO_GASOLIO:
                    ui->label_Nome_Allarme->setText(tr("Allarme filtro gasolio!"));
                    break;
                case ALLARME_GIRI_MOTORE:
                    ui->label_Nome_Allarme->setText(tr("Allarme giri motore!"));
                    break;
                case GUASTO_SENSORE_PRESSIONE:
                    ui->label_Nome_Allarme->setText(tr("Allarme sensore pressione!"));
                    break;
                case RISPARMIO_ENERGIA:
                    ui->label_Nome_Allarme->setText(tr("Risparmio energetico"));
                    break;
                case GUASTO_SENSORE_RECUPERO:
                    ui->label_Nome_Allarme->setText(tr("Allarme sensore recupero!"));
                    break;
                case ALLARME_RECUPERO:
                    ui->label_Nome_Allarme->setText(tr("Allarme recupero!"));
                    break;
                 default:
                    break;
                }
            }
            if (ListaECU.at(IndiceMacchinaAttiva)->Status & OFF_LINE){
            ui->PannelloMotoreRemoto->setEnabled(false);
            }
    }else{
        ui->label_NomeMacchina->setText(tr("Nessuna Macchina selezionata"));
        ui->label_UltimaLettura->setText(tr(""));
        ui->PannelloMotoreRemoto->setEnabled(false);
        ui->pushButton_Timer->setEnabled(false);
        ui->pushButton_Stop->setEnabled(false);
    }

    if (InitOk==false){
    //Resize dei bottoni timer e stop
    QSize IconSizes;
    IconSizes.setHeight(ui->pushButton_Stop->height());
    IconSizes.setWidth(ui->pushButton_Stop->height());
    ui->pushButton_Stop->setIconSize(IconSizes);
    ui->pushButton_Timer->setIconSize(IconSizes);

    QFont FontTimer=qApp->font();
 //   FontTimer.setPixelSize(ui->lineEdit_Timer_Minuti->height());
    FontTimer.setPixelSize((int)ui->pushButton_Stop->height()*0.6);
 //   ui->lineEdit_Timer_Minuti->setFont(FontTimer);
 //   ui->lineEdit_Timer_Ore->setFont(FontTimer);
//    ui->label_TimerMinuti->setFont(FontTimer);
//    ui->label_TimerOre->setFont(FontTimer);

    QFontMetrics FontMetrics(FontTimer);
    int FontWidth=FontMetrics.width((QChar)'0');
     Q_UNUSED(FontWidth);

 //   ui->lineEdit_Timer_Ore->setMaximumWidth(FontWidth*3);
 //   ui->lineEdit_Timer_Minuti->setMaximumWidth(FontWidth*3);

//    ui->label_TimerOre->setMaximumWidth(FontWidth*2);
//    FontWidth=FontMetrics.width((QChar)'m');
//    ui->label_TimerMinuti->setMaximumWidth(FontWidth*2);

    InitOk=true;
    }

    //Setta il focus per android
    ui->pushButton_Motore->setFocus();

}

void MainWindow::on_pushButton_Startec_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(2);
    //emit TurnOffMap();

    if ((ListaECU.at(IndiceMacchinaAttiva)->StatoStartec==0)&&((ListaECU.at(IndiceMacchinaAttiva)->EventoStartec==0)))
    activate_page_Allarmi_Startec();
    else activate_page_Startec();
}

void MainWindow::activate_page_Allarmi_Startec(){
    ui->stackedWidget_startec->setCurrentIndex(1); //Attiva la pagina allarmi startec
    //ridimensiona icona
        QSize IconSizes;
        IconSizes.setHeight((int)((qApp->screens().at(0)->geometry().width()/100)*20));
        IconSizes.setWidth((int)((qApp->screens().at(0)->geometry().width()/100)*20));
        ui->pushButton_Allarme_2->setIconSize(IconSizes);
}

void MainWindow::activate_page_Startec(){
    ui->stackedWidget_startec->setCurrentIndex(0); //Attiva la pagina normale startec
    if (ListaECU.count()){

        //Aggiorna i valori a seconda dello stato dei comandi startec in cui sei

        if (ListaECU.at(IndiceMacchinaAttiva)->ComandiStartecPending){
        int ValoriValidi=0;
           //Check Valore 1
            if ((ListaECU.at(IndiceMacchinaAttiva)->Vel_Imp_startec)==(ListaECU.at(IndiceMacchinaAttiva)->Vel_Imp_startecPending)){
                Vel_Imp_startec=(ListaECU.at(IndiceMacchinaAttiva))->Vel_Imp_startec;
                ValoriValidi++;
                emit updateTumblerVel_Imp_startec_bianco();
            }else{
                Vel_Imp_startec=(ListaECU.at(IndiceMacchinaAttiva))->Vel_Imp_startecPending;
                emit updateTumblerVel_Imp_startec_giallo();
            }
            //Check Valore 2
             if ((ListaECU.at(IndiceMacchinaAttiva)->Pausa_Start_startec)==(ListaECU.at(IndiceMacchinaAttiva)->Pausa_Start_startecPending)){
                 Pausa_Start_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_Start_startec;
                 ValoriValidi++;
                 emit updateTumblerPausa_Start_startec_bianco();
             }else{
                 Pausa_Start_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_Start_startecPending;
                 emit updateTumblerPausa_Start_startec_giallo();
             }
             //Check Valore 3
              if ((ListaECU.at(IndiceMacchinaAttiva)->Pausa_End_startec)==(ListaECU.at(IndiceMacchinaAttiva)->Pausa_End_startecPending)){
                  Pausa_End_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_End_startec;
                  ValoriValidi++;
                  emit updateTumblerPausa_End_startec_bianco();
              }else{
                  Pausa_End_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_End_startecPending;
                  emit updateTumblerPausa_End_startec_giallo();
              }

              if (ValoriValidi==3) ListaECU.at(IndiceMacchinaAttiva)->ComandiStartecPending=false;
        }else{
            Vel_Imp_startec=(ListaECU.at(IndiceMacchinaAttiva))->Vel_Imp_startec;
            Pausa_Start_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_Start_startec;
            Pausa_End_startec=(ListaECU.at(IndiceMacchinaAttiva))->Pausa_End_startec;
            emit updateTumblerVel_Imp_startec_bianco();
            emit updateTumblerPausa_Start_startec_bianco();
            emit updateTumblerPausa_End_startec_bianco();
        }

        emit updateTumblerVel_Imp_startec(Vel_Imp_startec/100,(Vel_Imp_startec%100)/10,(Vel_Imp_startec%100)%10);
        emit updateTumblerPausa_Start_startec(Pausa_Start_startec/100,((Pausa_Start_startec%100)/10),((Pausa_Start_startec%100)%10));
        emit updateTumblerPausa_End_startec(Pausa_End_startec/100,(Pausa_End_startec%100)/10,(Pausa_End_startec%100)%10);

         if (ListaECU.at(IndiceMacchinaAttiva)->StatoStartec<StatiStartec.count())  ui->lineEdit_stato_startec->setText(StatiStartec.at(ListaECU.at(IndiceMacchinaAttiva)->StatoStartec));
         else ui->lineEdit_stato_startec->setText("");
         if (((ListaECU.at(IndiceMacchinaAttiva)->EventoStartec)>=65)&& (ListaECU.at(IndiceMacchinaAttiva)->EventoStartec<(65+StatiStartec.count())))
             ui->lineEdit_evento_startec->setText(EventiStartec.at(ListaECU.at(IndiceMacchinaAttiva)->EventoStartec-65));
         else ui->lineEdit_evento_startec->setText("");

         ui->lineEdit_evento_startec_2->setText(tr("%1").arg(ListaECU.at(IndiceMacchinaAttiva)->LunghTuboStartec));
         ui->lineEdit_evento_startec_3->setText(tr("%1").arg(ListaECU.at(IndiceMacchinaAttiva)->VelMisStartec));
    }

static bool RidimensionaPulsante=true;

    if (RidimensionaPulsante){
        RidimensionaPulsante=false;
        QSize IconSizes;
        //resize bottone startec
        IconSizes.setHeight(ui->pushButton_Update_startec->height());
        IconSizes.setWidth(ui->pushButton_Update_startec->height());
        ui->pushButton_Update_startec->setIconSize(IconSizes);
    }
}

void MainWindow::on_pushButton_Mappa_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(3);
    //emit TurnOnMap();
    //CaricaMappa();
}

void MainWindow::on_pushButton_Wifi_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(4);
    //emit TurnOffMap();

 //   findActiveWirelesses();

//Esempio apertura sessione su periferica di default
//    // Set Internet Access Point
//    QNetworkConfigurationManager manager;
//    const bool canStartIAP = (manager.capabilities() & QNetworkConfigurationManager::CanStartAndStopInterfaces);

//    // Is there default access point, use it

//    QNetworkConfiguration cfg = manager.defaultConfiguration();
//    if (!cfg.isValid() || (!canStartIAP && cfg.state() != QNetworkConfiguration::Active)) {
//        QMessageBox::information(this, tr("Network"), tr("No Access Point found."));
//        return;
//    }

//    QNetworkSession* session = new QNetworkSession(cfg, this);
//    session->open();
//    session->waitForOpened(-1);
}

void MainWindow::Refresh_Lista_ECU(void){
//  salva l'indice
int Indice=-1;

    if (ui->listWidget_ECU->count()){
      Indice=ui->listWidget_ECU->currentRow();
    }else{
        Indice=0;
        IndiceMacchinaAttiva=0;
    }

//  Svuota il listview
     ui->listWidget_ECU->clear();

 //aggiungi gli elementi della lista
       int Index=0;
       foreach(ECU_t* Elemento, ListaECU){
           QListWidgetItem* item;
           item = new QListWidgetItem(ui->listWidget_ECU);
           WidgetECU* NewWidgetECU=new WidgetECU(this,Elemento);

           QSize Size;
           Size.setHeight(NewWidgetECU->height);
           Size.setWidth(ui->listWidget_ECU->width()-4);
           item->setSizeHint(Size);

 //          ui->listWidget_ECU->setGridSize(QSize(ui->listWidget_ECU->width()-4,NewWidgetECU->height));
           ui->listWidget_ECU->setItemWidget(item, NewWidgetECU);
           ui->listWidget_ECU->addItem(item);
           Index++;
       }

//Se ho almeno un elemento lo seleziono
       if (ui->listWidget_ECU->count()){
           if (IndiceMacchinaAttiva>=0){
              ((WidgetECU*)ui->listWidget_ECU->itemWidget(ui->listWidget_ECU->item(IndiceMacchinaAttiva)))->setStyleSheet("background-color: rgb(255, 255, 255);");
            }
           ui->listWidget_ECU->setCurrentRow(Indice);
            IndiceMacchinaAttiva=ui->listWidget_ECU->currentIndex().row();
            ((WidgetECU*)ui->listWidget_ECU->itemWidget(ui->listWidget_ECU->item(IndiceMacchinaAttiva)))->setStyleSheet("background-color: rgb(0, 255, 255);");

            if (ListaECU.at(IndiceMacchinaAttiva)->Descrizione.section(' ', 0, 0, QString::SectionSkipEmpty)==tr("GMP")){
                ui->pushButton_Startec->setEnabled(false);
            }else  ui->pushButton_Startec->setEnabled(true);
       }
}


void MainWindow::on_listWidget_ECU_itemClicked(QListWidgetItem *item)
{
   Q_UNUSED(item);
   if (IndiceMacchinaAttiva>=0){
      ((WidgetECU*)ui->listWidget_ECU->itemWidget(ui->listWidget_ECU->item(IndiceMacchinaAttiva)))->setStyleSheet("background-color: rgb(255, 255, 255);");
    }

    IndiceMacchinaAttiva=ui->listWidget_ECU->currentIndex().row();
    ((WidgetECU*)ui->listWidget_ECU->itemWidget(ui->listWidget_ECU->item(IndiceMacchinaAttiva)))->setStyleSheet("background-color: rgb(0, 255, 255);");

    if (ListaECU.at(IndiceMacchinaAttiva)->Descrizione.section(' ', 0, 0, QString::SectionSkipEmpty)==tr("GMP")){
        ui->pushButton_Startec->setEnabled(false);
    }else  ui->pushButton_Startec->setEnabled(true);

    emit ui->pushButton_Motore->click();
}

void MainWindow::InterpreteRispostaHttp(QString replyTemp){
    qDebug() << "Dati arrivati!";
//    QString SPicchetti = LineaBuona.section(QRegExp("\\s+"), 0, 0, QString::SectionSkipEmpty);
    switch(hHTTP.ComandoAttuale){
        case DOWNLOAD_STATO_MACCHINA:
            if((replyTemp.left(2))==QString("GO")){
                ECU_t* ECU=ListaECU.at(hHTTP.IndiceMacchinaComando);
                //Tutto ok procedi
                if (replyTemp.mid(2,10)==ECU->NumeroSerialeSW){
                    //Macchina giusta, procedi
                    QString UltimaLettura=replyTemp.section('|', 1, 1, QString::SectionSkipEmpty);
                    ECU->UltimaLettura=QDateTime::fromString(UltimaLettura,"yyyy-MM-dd hh:mm:ss");
                    ECU->GiriMotore=replyTemp.section('|', 2, 2, QString::SectionSkipEmpty).toInt();
                    ECU->Pressione=replyTemp.section('|', 3, 3, QString::SectionSkipEmpty).toFloat();
                    ECU->Depressione=replyTemp.section('|', 4, 4, QString::SectionSkipEmpty).toFloat();
                    ECU->PressRec=replyTemp.section('|', 5, 5, QString::SectionSkipEmpty).toInt();
                    ECU->PressRecImp=replyTemp.section('|', 6, 6, QString::SectionSkipEmpty).toInt();
                    ECU->Gasolio=replyTemp.section('|', 7, 7, QString::SectionSkipEmpty).toInt();
                    ECU->TempH2O=replyTemp.section('|', 8, 8, QString::SectionSkipEmpty).toInt();
                    ECU->VBat=replyTemp.section('|', 9, 9, QString::SectionSkipEmpty).toFloat();
                    ECU->StatoMotore=(ECU_Motor_status)replyTemp.section('|', 10, 10, QString::SectionSkipEmpty).toInt();
                    ECU->Latitudine=replyTemp.section('|', 18, 18, QString::SectionSkipEmpty).toDouble();
                    ECU->Longitudine=replyTemp.section('|', 19, 19, QString::SectionSkipEmpty).toDouble();
                    int AppoggioStatoStartec=replyTemp.section('|', 11, 11, QString::SectionSkipEmpty).toInt();
                    ECU->StatoStartec=(quint8)(AppoggioStatoStartec/1000);
                    ECU->EventoStartec=(quint8)(AppoggioStatoStartec%1000);
                    ECU->LunghTuboStartec=(quint16)replyTemp.section('|', 12, 12, QString::SectionSkipEmpty).toInt();
                    ECU->VelMisStartec=(quint16)replyTemp.section('|', 13, 13, QString::SectionSkipEmpty).toInt();
                    ECU->Vel_Imp_startec=replyTemp.section('|', 14, 14, QString::SectionSkipEmpty).toInt();
                    ECU->Pausa_Start_startec=replyTemp.section('|', 15, 15, QString::SectionSkipEmpty).toInt();
                    ECU->Pausa_End_startec=replyTemp.section('|', 16, 16, QString::SectionSkipEmpty).toInt();

                    //Check dell'ora per decidere se è online o offline
                    QDateTime now = QDateTime::currentDateTime();
                    ECU_Status     StatusOld;

                    StatusOld=ECU->Status;
                    ECU->Status=ON_LINE;

                    if (ECU->UltimaLettura.secsTo(now)>SECONDI_OFFLINE){
                     ECU->Status=(ECU_Status)((char)ECU->Status|(char)OFF_LINE);
                    }

                    if ((ECU->StatoMotore==FINE_LAVORO)||(ECU->StatoMotore==RISPARMIO_ENERGIA)){
                        ECU->Status=(ECU_Status)((char)ECU->Status|(char)MACHINE_WARNING);
                    }
                    else if (ECU->StatoMotore!=ALLARME_NO){
                        ECU->Status=(ECU_Status)((char)ECU->Status|(char)MACHINE_ERROR);
                    }

                     #ifdef Q_OS_ANDROID
                    //Se la macchina è in errore genera la notifica
                    if(((StatusOld&(char)MACHINE_ERROR)==0)&&((ECU->Status&(char)MACHINE_ERROR))&&(!(ECU->Status&(char)OFF_LINE))){
                            if (NotificationID<1000) NotificationID++;
                            notificationClient->setNotification(tr("Alarm %1!").arg(ECU->Nome),NotificationID);
                    }else{
                        //Se la macchina è in warning genera la notifica
                        if(((StatusOld&(char)MACHINE_WARNING)==0)&&((ECU->Status&(char)MACHINE_WARNING))&&(!(ECU->Status&(char)OFF_LINE))){
                             if (NotificationID<1000) NotificationID++;
                             notificationClient->setNotificationWarning(tr("Warning %1!").arg(ECU->Nome),NotificationID);
                        }
                    }
                    #elif defined Q_OS_IOS
                    //Se la macchina è in errore genera la notifica
                    if(((StatusOld&(char)MACHINE_ERROR)==0)&&((ECU->Status&(char)MACHINE_ERROR))&&(!(ECU->Status&(char)OFF_LINE))){
                            if (NotificationID<1000) NotificationID++;
                            qDebug() << "Send notification!";
                            QtIosNotifier* Notifier=new(QtIosNotifier);
                            Notifier->show(tr("Alarm:"),tr("%1!").arg(ECU->Nome),NotificationID);
                            //notificationClient->setNotification(tr("Alarm %1!").arg(ECU->Nome),NotificationID);
                    }else{
                        //Se la macchina è in warning genera la notifica
                        if(((StatusOld&(char)MACHINE_WARNING)==0)&&((ECU->Status&(char)MACHINE_WARNING))&&(!(ECU->Status&(char)OFF_LINE))){
                             if (NotificationID<1000) NotificationID++;
                             qDebug() << "Send notification!";
                             QtIosNotifier* Notifier=new(QtIosNotifier);
                             Notifier->show(tr("Warning:"),tr("%1!").arg(ECU->Nome),NotificationID);
                          //   notificationClient->setNotificationWarning(tr("Warning %1!").arg(ECU->Nome),NotificationID);
                        }
                    }

                    #endif
                    hHTTP.Result=HTTP_OK;
                }
            }
            break;

       case DOWNLOAD_ELENCO_MACCHINE:
        if((replyTemp.left(2))==QString("GO")){
            QString Go=replyTemp.section('|', 0, 0, QString::SectionSkipEmpty);
            quint8 NumeroMacchine=(Go.remove(0,2).toInt());
            QString NumeriSerie=replyTemp.section('|', 1, 1, QString::SectionSkipEmpty);
            //Tutto ok procedi
                for(quint8 i=0; i<NumeroMacchine; i++){
                    QString NumeroSerie=NumeriSerie.section('*', i, i, QString::SectionSkipEmpty);
                    //Creo una lista ECU di prova
                        ECU_t* NewECU=new ECU_t;
                        NewECU->Nome=tr("");
                        NewECU->NumeroSerialeSW=NumeroSerie;
                        NewECU->Descrizione=tr("");
                        NewECU->Status=OFF_LINE;
                        NewECU->GiriMotore=0;
                        NewECU->Pressione=0;
                        NewECU->Depressione=0;
                        NewECU->TempH2O=0;
                        NewECU->VBat=0;
                        NewECU->PressRec=0;
                        NewECU->PressRecImp=0;
                        NewECU->Gasolio=0;
                        NewECU->StatoMotore=ALLARME_NO;
                        NewECU->ComandiStartecPending=false;
                        ListaECU.append(NewECU);
                }
            hHTTP.Result=HTTP_OK;
        }
            break;

        case DOWNLOAD_DETTAGLI_MACCHINE:
        if((replyTemp.left(2))==QString("GO")){
            ECU_t* ECU=ListaECU.at(hHTTP.IndiceMacchinaComando);
            //Tutto ok procedi
            if (replyTemp.mid(2,10)==ECU->NumeroSerialeSW){
                //Macchina giusta, procedi
                ECU->NumeroSerialeHW=replyTemp.section('|', 1, 1, QString::SectionSkipEmpty);
                ECU->NumeroSIM=replyTemp.section('|', 2, 2, QString::SectionSkipEmpty);
                ECU->Nome=replyTemp.section('|', 3, 3, QString::SectionSkipEmpty);
                ECU->Descrizione=replyTemp.section('|', 4, 4, QString::SectionSkipEmpty);
                if (!ECU->Descrizione.contains("GMP")){
                ECU->Descrizione=replyTemp.section('|', 4, 4, QString::SectionSkipEmpty)+tr(" ")+replyTemp.section('|', 5, 5, QString::SectionSkipEmpty)+tr("-")+replyTemp.section('|', 6, 6, QString::SectionSkipEmpty);
                }
                hHTTP.Result=HTTP_OK;
            }
        }
        break;

        default:

            break;
    }
}

void MainWindow::InterpreteRispostaHttpAsync(QString replyTemp){
    switch(hHTTPAsync.ComandoAttuale){
        case UPLOAD_COMANDO_MACCHINA:
            if((replyTemp.left(2))==QString("GO")){
                //upload comando ok
                ECU_t* ECU=ListaECU.at(hHTTPAsync.IndiceMacchinaComando);
                ECU->StatusCommand=QString("1");
                ECU->ComandiStartecPending=true;
            }else {
                //errore upload comando
                ECU_t* ECU=ListaECU.at(hHTTPAsync.IndiceMacchinaComando);
                ECU->StatusCommand=QString("99"); //segnala errore
            }
            break;

        case DOWNLOAD_COMANDO_MACCHINA:
                if((replyTemp.left(2))==QString("GO")){
                    if (hHTTPAsync.IndiceMacchinaComando>-1){
                        ECU_t* ECU=ListaECU.at(hHTTPAsync.IndiceMacchinaComando);
                        //Tutto ok procedi
                        if (replyTemp.mid(2,10)==ECU->NumeroSerialeSW){
                        ECU->ComandoPending=replyTemp.section('|', 2, 2, QString::SectionSkipEmpty);
                        ECU->StatusCommand=replyTemp.section('|', 3, 3, QString::SectionSkipEmpty);

                        int ValStatusCommand=ECU->StatusCommand.toInt();
                        if (ValStatusCommand>1){
                            TipoComandoAsync=NO_COMMAND;
                            CheckComandoEseguito=false;
                        }

                        UpdateTastiTimerStop();

    //                        //Macchina giusta, procedi
    //                        QString UltimaLettura=reply.section('|', 1, 1, QString::SectionSkipEmpty);
    //                        ECU->UltimaLettura=QDateTime::fromString(UltimaLettura,"yyyy-MM-dd hh:mm:ss");
    //                        ECU->GiriMotore=reply.section('|', 2, 2, QString::SectionSkipEmpty).toInt();
    //                        ECU->Pressione=reply.section('|', 3, 3, QString::SectionSkipEmpty).toFloat();
    //                        ECU->Depressione=reply.section('|', 4, 4, QString::SectionSkipEmpty).toFloat();
    //                        ECU->PressRec=reply.section('|', 5, 5, QString::SectionSkipEmpty).toInt();
    //                        ECU->PressRecImp=reply.section('|', 6, 6, QString::SectionSkipEmpty).toInt();
    //                        ECU->Gasolio=reply.section('|', 7, 7, QString::SectionSkipEmpty).toInt();
    //                        ECU->TempH2O=reply.section('|', 8, 8, QString::SectionSkipEmpty).toInt();
    //                        ECU->VBat=reply.section('|', 9, 9, QString::SectionSkipEmpty).toFloat();
    //                        ECU->StatoMotore=(ECU_Motor_status)reply.section('|', 10, 10, QString::SectionSkipEmpty).toInt();

    //                        //Check dell'ora per decidere se è online o offline
    //                        QDateTime now = QDateTime::currentDateTime();

    //                        ECU->Status=ON_LINE;

    //                        if (ECU->UltimaLettura.secsTo(now)>SECONDI_OFFLINE){
    //                         ECU->Status=(ECU_Status)((char)ECU->Status|(char)OFF_LINE);
    //                        }

    //                        if ((ECU->StatoMotore==FINE_LAVORO)||(ECU->StatoMotore==RISPARMIO_ENERGIA)){
    //                            ECU->Status=(ECU_Status)((char)ECU->Status|(char)MACHINE_WARNING);
    //                        }
    //                        else if (ECU->StatoMotore!=ALLARME_NO){
    //                            ECU->Status=(ECU_Status)((char)ECU->Status|(char)MACHINE_ERROR);
    //                        }

                            hHTTPAsync.Result=HTTP_OK;
                        }
                   }
                }else{
                    ECU_t* ECU=ListaECU.at(hHTTPAsync.IndiceMacchinaComando);
                    ECU->StatusCommand=QString("99"); //se arriva ko segnala errore
                }
            break;
        default:

            break;
    }
}


void MainWindow::on_pushButton_Riprova_clicked()
{
        RichiediElencoMacchine();
        //InviaComandoHTTP(DOWNLOAD_STATO_MACCHINA, 0);
}

void MainWindow::CallbackBaseTempi(void){
    if (TimerTimeoutConnessione>1) TimerTimeoutConnessione--;
    else if (TimerTimeoutConnessione==1){
        TimerTimeoutConnessione--;
        ShowTerminalErrorMessage(ui->textEdit,tr("ERROR: HTTP Timeout!"));
        reply->deleteLater();
        reply = Q_NULLPTR;
        hHTTP.ComandoAttuale=NESSUN_COMANDO;
        hHTTP.Result=HTTP_ERROR;
        ui->stackedWidget_ElencoMacchine->setCurrentIndex(1);
        return;
    }

    if (TimerTimeoutConnessioneAsync>1) TimerTimeoutConnessioneAsync--;
    else if (TimerTimeoutConnessioneAsync==1){
        TimerTimeoutConnessioneAsync--;
        ShowTerminalErrorMessage(ui->textEdit,tr("URLAsync ERROR: HTTP Timeout!"));
        replyAsync->deleteLater();
        replyAsync = Q_NULLPTR;
        hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
        hHTTPAsync.Result=HTTP_ERROR;
        ui->stackedWidget_ElencoMacchine->setCurrentIndex(1);
        return;
    }

//    if (CheckComandoEseguito){
//        if (PrescalerComando) PrescalerComando--;
//        else {
//            PrescalerComando=SECONDI_REFRESH_COMANDO;
//            //Update stato del comando delle macchine
//             if (hHTTPAsync.ComandoAttuale==NESSUN_COMANDO) InviaComandoHTTP_Async(DOWNLOAD_COMANDO_MACCHINA, IndiceMacchinaComandoAsync);
//        }
//    }else{
//    PrescalerComando=SECONDI_REFRESH_COMANDO;
//    }

    if (PrescalerStato) PrescalerStato--;
    else {
        PrescalerStato=SECONDI_REFRESH_STATO;

        //Update stato delle macchine
         if ((hHTTP.ComandoAttuale==NESSUN_COMANDO)&&(ListaECU.count())){
             InviaComandoHTTP(DOWNLOAD_STATO_MACCHINA, 0);
            //Update stato del comando delle macchine
            if (hHTTPAsync.ComandoAttuale==NESSUN_COMANDO) InviaComandoHTTP_Async(DOWNLOAD_COMANDO_MACCHINA, 0);
         }
    }
}


void MainWindow::stateChanged(Qt::ApplicationState newState) {
    if (newState==Qt::ApplicationSuspended){
     setUpdatesEnabled(false);
    }
    else{
     if (updatesEnabled()==false){
         setUpdatesEnabled(true);
         this->update();
         }
    }
    QApplication::processEvents();
}

void MainWindow::RichiediElencoMacchine(void){
ListaECU.clear();
ComandiUpdated=false;
ui->stackedWidget_ElencoMacchine->setCurrentIndex(ui->stackedWidget_ElencoMacchine->indexOf(ui->page_Wait));
qDebug() << "DEBUG: Richiedi elenco macchine";
if (hHTTP.ComandoAttuale!=NESSUN_COMANDO){
    qDebug() << "DEBUG: comando attuale diverso da nessun comando" ;

    if (reply!=Q_NULLPTR){
        if (reply->isRunning()){
        reply->abort();
        reply->deleteLater();
        reply = Q_NULLPTR;
        }
    }
    hHTTP.ComandoAttuale=NESSUN_COMANDO;
}

if (hHTTPAsync.ComandoAttuale!=NESSUN_COMANDO){
     qDebug() << "DEBUG: comando attuale ASYNC diverso da nessun comando" ;
    if (replyAsync!=Q_NULLPTR){
        if (replyAsync->isRunning()){
        replyAsync->abort();
        replyAsync->deleteLater();
        replyAsync = Q_NULLPTR;
        }
    }
    hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
}

 qDebug() << "DEBUG: invia elenco macchine" ;
InviaComandoHTTP(DOWNLOAD_ELENCO_MACCHINE, 0);
}

void MainWindow::InviaComandoHTTP(Comandi_Server_t  ComandoAttuale,int IndiceMacchina){
// Richiedi stato della macchina al server
QString StringaComando;

if (hHTTP.ComandoAttuale==NESSUN_COMANDO) hHTTP.Retry=3;
else{
    if (hHTTP.Retry) hHTTP.Retry--;
    else{
        hHTTP.Result=HTTP_ERROR;
        hHTTP.ComandoAttuale=NESSUN_COMANDO;
        return;
    }
}

hHTTP.ComandoAttuale=ComandoAttuale;
hHTTP.IndiceMacchinaComando=IndiceMacchina;
hHTTP.Result=HTTP_NONE;

QString MD5String;

//Componi la stringa a seconda del comando
    switch (ComandoAttuale){
        case DOWNLOAD_ELENCO_MACCHINE:
        MD5String = QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
        StringaComando=tr("http://www.nomeserver.com/11.php?01=%1&02=%2").arg(UserName).arg(MD5String);
        break;

        case DOWNLOAD_DETTAGLI_MACCHINE:
        MD5String = QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
        StringaComando=tr("http://www.nomeserver.com/12.php?01=%1&02=%2&03=%3").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTP.IndiceMacchinaComando)->NumeroSerialeSW);
        break;

        case DOWNLOAD_STATO_MACCHINA:
        MD5String = QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
        StringaComando=tr("http://www.nomeserver.com/02.php?01=%1&02=%2&03=%3").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTP.IndiceMacchinaComando)->NumeroSerialeSW);
        break;

        default:
        break;
    }

QUrl url=QUrl::fromUserInput(StringaComando);
startRequest(url);
//startRequest_Async(url);
}

void MainWindow::InviaComandoHTTP_Async(Comandi_Server_t  ComandoAttuale,int IndiceMacchina){
    if (ListaECU.count()>IndiceMacchina){
    // Richiedi stato della macchina al server

    QString StringaComando;

    if (hHTTPAsync.ComandoAttuale==NESSUN_COMANDO) hHTTPAsync.Retry=3;
    else{
        if (hHTTPAsync.Retry) hHTTPAsync.Retry--;
        else{
            hHTTPAsync.Result=HTTP_ERROR;
            hHTTPAsync.ComandoAttuale=NESSUN_COMANDO;
            return;
        }
    }

    hHTTPAsync.ComandoAttuale=ComandoAttuale;
    hHTTPAsync.IndiceMacchinaComando=IndiceMacchina;
    hHTTPAsync.Result=HTTP_NONE;

    QString MD5String;
    QString InputMD5;
    QString OutputMD5;

    //Componi la stringa a seconda del comando
        switch (ComandoAttuale){
            case UPLOAD_COMANDO_MACCHINA:
                switch(TipoComandoAsync){
                case STOP:
                        StringaUltimoComando=QString("STOP");
                    break;
                case TIMER:
                        StringaUltimoComando=QString("T%1%2").arg(TimerOre,2,10,QChar('0')).arg(TimerMinuti,2,10,QChar('0'));
                    break;
                case STARTEC:
                        StringaUltimoComando=QString("ST_V%1I%2F%3").arg(Vel_Imp_startec,3,10,QChar('0')).arg(Pausa_Start_startec,3,10,QChar('0')).arg(Pausa_End_startec,3,10,QChar('0'));
                        ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->Vel_Imp_startecPending=Vel_Imp_startec;
                        ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->Pausa_Start_startecPending=Pausa_Start_startec;
                        ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->Pausa_End_startecPending=Pausa_End_startec;
                    break;
                default:
                    break;
                }
            MD5String = QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
            InputMD5=tr("%1%2%3%4%5%6").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->NumeroSerialeSW).arg("0").arg(StringaUltimoComando).arg("1");
            OutputMD5 = QString(QCryptographicHash::hash(InputMD5.toUtf8(),QCryptographicHash::Md5).toHex());
            StringaComando=tr("http://www.nomeserver.com/13.php?01=%1&02=%2&03=%3&04=%4&05=%5&06=%6&07=%7").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->NumeroSerialeSW).arg("0").arg(StringaUltimoComando).arg("1").arg(OutputMD5);
            break;

            case DOWNLOAD_COMANDO_MACCHINA:
            MD5String = QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
            InputMD5=tr("%1%2%3%4%5%6").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->NumeroSerialeSW).arg("0").arg(StringaUltimoComando).arg("1");
            OutputMD5 = QString(QCryptographicHash::hash(InputMD5.toUtf8(),QCryptographicHash::Md5).toHex());
            StringaComando=tr("http://www.nomeserver.com/15.php?01=%1&02=%2&03=%3&04=%4").arg(UserName).arg(MD5String).arg(ListaECU.at(hHTTPAsync.IndiceMacchinaComando)->NumeroSerialeSW).arg("0");
            break;
            default:
            break;
        }

    QUrl url=QUrl::fromUserInput(StringaComando);
    startRequest_Async(url);
    }
}



void MainWindow::on_pushButton_User_clicked()
{
    ui->label_errore_credenziali->setText(tr(""));
    if (ui->pushButton_User->isChecked()){
        ui->lineEdit_Username->setText(UserName);
        ui->lineEdit_Password->setText(Password);
        ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Login));
    }else{
        if (ListaECU.count()){
            ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Core));
//                ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_3));
        }else{
            ui->pushButton_User->setChecked(true);
        }
    }
}

void MainWindow::on_pushButton_Login_clicked()
{
    UserName=ui->lineEdit_Username->text();
    Password=ui->lineEdit_Password->text();
    QSettings settings (ProjectPath.absolutePath()+QString("/Settings.ini"), QSettings::IniFormat);
    settings.beginGroup("User_Settings");
    settings.setValue("UserName",UserName);
    settings.setValue("Password",Password);
    settings.endGroup();
    RichiediElencoMacchine();
}

void MainWindow::on_pushButton_Stop_clicked()
{
    DisabilitaComandi();
    ui->pushButton_Stop->setStyleSheet(BOTTONE_GIALLO);

    hHTTPAsync.IndiceMacchinaComando=IndiceMacchinaAttiva;
    TipoComandoAsync=STOP;
    ListaECU.at(IndiceMacchinaAttiva)->StatusCommand=QString("0");
    InviaComandoHTTP_Async(UPLOAD_COMANDO_MACCHINA,IndiceMacchinaAttiva);
}

void MainWindow::on_pushButton_Timer_clicked()
{
    DisabilitaComandi();
    ui->pushButton_Timer->setStyleSheet(BOTTONE_GIALLO);

    hHTTPAsync.IndiceMacchinaComando=IndiceMacchinaAttiva;
    TipoComandoAsync=TIMER;
    ListaECU.at(IndiceMacchinaAttiva)->StatusCommand=QString("0");
    InviaComandoHTTP_Async(UPLOAD_COMANDO_MACCHINA,IndiceMacchinaAttiva);
}

//bool MainWindow::eventFilter(QObject *object, QEvent *event)
//{
//    if (event->type() == QEvent::FocusIn)
//    {
//        if (object == ui->lineEdit_Timer_Ore)
//        {
//  //          ui->lineEdit_Timer_Ore->clear();
//  //          ui->lineEdit_Timer_Ore->setCursorPosition(0);

////             // First let the base class process the event
////             QLineEdit::focusInEvent(event);
//             // Then select the text by a single shot timer, so that everything will
//             // be processed before (calling selectAll() directly won't work)

//             TimerLineditOre.singleShot(100, this,SLOT(Callback_Timer_Ore_SelectAll()));

//        }else if (object == ui->lineEdit_Timer_Minuti)
//        {
//  //          ui->lineEdit_Timer_Minuti->clear();
//  //          ui->lineEdit_Timer_Minuti->setCursorPosition(0);
//            TimerLineditMinuti.singleShot(100, this,SLOT(Callback_Timer_Minuti_SelectAll()));
//        }
//    }else  if (event->type() == QEvent::FocusOut)
//    {
//        if (object == ui->lineEdit_Timer_Ore)
//        {
//            ui->lineEdit_Timer_Ore->setText(QString("%1").arg(TimerOre, 2, 10, QChar('0')));
//        }else if (object == ui->lineEdit_Timer_Minuti)
//        {
//            ui->lineEdit_Timer_Minuti->setText(QString("%1").arg(TimerMinuti, 2, 10, QChar('0')));
//        }
//    }else if (event->type()==QEvent::KeyPress) {
//        QKeyEvent* key = static_cast<QKeyEvent*>(event);
//        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
//            //Enter or return was pressed
//            if (ui->lineEdit_Timer_Minuti->isModified()){
//                on_lineEdit_Timer_Minuti_returnPressed();
//            }else if  (ui->lineEdit_Timer_Ore->isModified()){
//                on_lineEdit_Timer_Ore_returnPressed();
//            }
//        }
//    }
//    return false;
//}

//void MainWindow::on_lineEdit_Timer_Ore_returnPressed()
//{
//    TimerOre=ui->lineEdit_Timer_Ore->text().toInt();
//    ui->lineEdit_Timer_Ore->setText(QString("%1").arg(TimerOre, 2, 10, QChar('0')));
//    emit ui->pushButton_Motore->clicked();
//    emit ui->lineEdit_Timer_Ore->editingFinished();
//}



//void MainWindow::on_lineEdit_Timer_Minuti_returnPressed()
//{
//    TimerMinuti=ui->lineEdit_Timer_Minuti->text().toInt();
//    ui->lineEdit_Timer_Minuti->setText(QString("%1").arg(TimerMinuti, 2, 10, QChar('0')));
//    emit ui->pushButton_Motore->clicked();
//    emit ui->lineEdit_Timer_Minuti->editingFinished();
//}

void MainWindow::closeEvent(QCloseEvent *event){
#ifdef Q_OS_ANDROID
   qDebug()<<"APPLEBY: Close Event!";
   event->ignore();
   emit ui->pushButton_Elenco->click();
   ui->pushButton_Elenco->setFocus();
   this->update();
   QApplication::processEvents();

   notificationClient->GoToBackground();
#elif defined Q_OS_IOS
    qDebug()<<"APPLEBY: Close Event!";
    event->ignore();
    emit ui->pushButton_Elenco->click();
    ui->pushButton_Elenco->setFocus();
    this->update();
    QApplication::processEvents();
#endif

}



void MainWindow::UpdateTastiTimerStop(void){
//decidi come gestire i tasti stop e timer
ECU_t* ECU=ListaECU.at(IndiceMacchinaAttiva);


    if ((ECU->Status & OFF_LINE)||(ECU->Status & MACHINE_ERROR)||(ECU->Status & MACHINE_WARNING)||(ComandiUpdated==false)){
            DisabilitaComandi();
    }else{
    int ValStatusCommand=ECU->StatusCommand.toInt();
        switch (ValStatusCommand){
            //comando pending
            case 1:
                DisabilitaComandi();

                if (ECU->ComandoPending=="STOP"){
                    ui->pushButton_Stop->setStyleSheet(BOTTONE_GIALLO);
                }else if (ECU->ComandoPending.left(3)=="ST_"){
                    ui->pushButton_Update_startec->setStyleSheet(BOTTONE_GIALLO);
                }else if  (ECU->ComandoPending.left(1)=="T"){
                    ui->pushButton_Timer->setStyleSheet(BOTTONE_GIALLO);
                }
            break;

            //comando eseguito
            case 2:                
                AbilitaComandi();

                if (ECU->ComandoPending=="STOP"){
                    ui->pushButton_Stop->setStyleSheet(BOTTONE_VERDE);
                }else if (ECU->ComandoPending.left(3)=="ST_"){
                    ui->pushButton_Update_startec->setStyleSheet(BOTTONE_VERDE);
                }else  if (ECU->ComandoPending.left(1)=="T"){
                    ui->pushButton_Timer->setStyleSheet(BOTTONE_VERDE);
                }
            break;

            //errore
            case 99:
                    AbilitaComandi();

                    if (ECU->ComandoPending=="STOP"){
                        ui->pushButton_Stop->setStyleSheet(BOTTONE_ROSSO);
                    }else if (ECU->ComandoPending.left(3)=="ST_"){
                        ui->pushButton_Update_startec->setStyleSheet(BOTTONE_ROSSO);
                    }else if (ECU->ComandoPending.left(1)=="T"){
                        ui->pushButton_Timer->setStyleSheet(BOTTONE_ROSSO);
                    }
            break;
            default:

            break;
        }
    }
}

//void MainWindow::Callback_Timer_Ore_SelectAll(void){
//    ui->lineEdit_Timer_Ore->selectAll();
//}

//void MainWindow::Callback_Timer_Minuti_SelectAll(void){
//    ui->lineEdit_Timer_Minuti->selectAll();
//}



//void MainWindow::on_lineEdit_Timer_Ore_editingFinished()
//{
//    ui->lineEdit_Timer_Ore->setText(QString("%1").arg(TimerOre, 2, 10, QChar('0')));
//}


//void MainWindow::on_lineEdit_Timer_Minuti_editingFinished()
//{
//    ui->lineEdit_Timer_Minuti->setText(QString("%1").arg(TimerMinuti, 2, 10, QChar('0')));
//}



//void MainWindow::RicaricaMappa(void){
// //   emit ui->pushButton_Mappa->click();
////    FileHTML.clear();
////    FileHTML=FileHTMLModel;
////    QString Coordinate=tr("(%1,%2)").arg((double)(ListaECU.at(IndiceMacchinaAttiva))->Latitudine).arg((double)(ListaECU.at(IndiceMacchinaAttiva))->Longitudine);
////    FileHTML.replace("(44.9016223,9.851193599999988)",Coordinate);
////    ContextMappa->setContextProperty("MainWindow", this);
////    emit FileHTMLChanged();
////    QApplication::processEvents();
////    qDebug()<<"Coordinate aggiornate:" << Coordinate;
//    emit ReloadMap();
//    QApplication::processEvents();
//}

//void MainWindow::RischiacciaBottoneMappa(void){
//    emit ui->pushButton_Mappa->click();
//}

 
void MainWindow::on_stackedWidget_App_currentChanged(int arg1)
{
    if (arg1!=3){
        //distruggi il widget
        if (Mappa!=nullptr){
            delete Mappa;
            Mappa=nullptr;
        }
    }else{
        if (ListaECU.count()){
            //crea e visualizza il widget
            Mappa= new MapWidget(ui->frame_Mappa->width(), ui->frame_Mappa->height(), (double)(ListaECU.at(IndiceMacchinaAttiva))->Latitudine,(double)(ListaECU.at(IndiceMacchinaAttiva))->Longitudine, this);

            //set layout programatically
            ui->frame_Mappa->layout()->addWidget(Mappa);
        }
    }
     //   RicaricaMappaAndroid=1; //riarma il caricamento doppio della mappa per android
}

void MainWindow::VelImpStartec_Item_cifra_changed(int Position, int Value){
    if (ListaECU.count()){
        int Vel_Imp_startec_Centinaia= Vel_Imp_startec/100;
        int Vel_Imp_startec_Decine= (Vel_Imp_startec%100)/10;
        int Vel_Imp_startec_Unita= (Vel_Imp_startec%100)%10;

            if (Position==1){
                Vel_Imp_startec_Unita=Value;
            }else if  (Position==2){
                Vel_Imp_startec_Decine=Value;
            }else if  (Position==3){
                Vel_Imp_startec_Centinaia=Value;
            }
        Vel_Imp_startec=(Vel_Imp_startec_Centinaia*100)+(Vel_Imp_startec_Decine*10)+Vel_Imp_startec_Unita;

        if (Vel_Imp_startec<5){
            Vel_Imp_startec=5;
            emit updateTumblerVel_Imp_startec(Vel_Imp_startec/100,(Vel_Imp_startec%100)/10,(Vel_Imp_startec%100)%10);
        }
    }
}

void MainWindow::PausaStartStartec_Item_cifra_changed(int Position, int Value){
     if (ListaECU.count()){
        int Vel_Imp_startec_Centinaia= Pausa_Start_startec/100;
        int Vel_Imp_startec_Decine= (Pausa_Start_startec%100)/10;
        int Vel_Imp_startec_Unita= (Pausa_Start_startec%100)%10;

            if (Position==1){
                Vel_Imp_startec_Unita=Value;
            }else if  (Position==2){
                Vel_Imp_startec_Decine=Value;
            }else if  (Position==3){
                Vel_Imp_startec_Centinaia=Value;
            }
      Pausa_Start_startec=(Vel_Imp_startec_Centinaia*100)+(Vel_Imp_startec_Decine*10)+Vel_Imp_startec_Unita;
      if (Pausa_Start_startec>250){
          Pausa_Start_startec=250;
          emit updateTumblerPausa_Start_startec(Pausa_Start_startec/100,(Pausa_Start_startec%100)/10,(Pausa_Start_startec%100)%10);
      }
    }
}

void MainWindow::PausaEndStartec_Item_cifra_changed(int Position, int Value){
      if (ListaECU.count()){
        int Vel_Imp_startec_Centinaia= Pausa_End_startec/100;
        int Vel_Imp_startec_Decine= (Pausa_End_startec%100)/10;
        int Vel_Imp_startec_Unita= (Pausa_End_startec%100)%10;

            if (Position==1){
                Vel_Imp_startec_Unita=Value;
            }else if  (Position==2){
                Vel_Imp_startec_Decine=Value;
            }else if  (Position==3){
                Vel_Imp_startec_Centinaia=Value;
            }
        Pausa_End_startec=(Vel_Imp_startec_Centinaia*100)+(Vel_Imp_startec_Decine*10)+Vel_Imp_startec_Unita;
        if (Pausa_End_startec>250){
            Pausa_End_startec=250;
            emit updateTumblerPausa_End_startec(Pausa_End_startec/100,(Pausa_End_startec%100)/10,(Pausa_End_startec%100)%10);
        }
    }
}


void MainWindow::on_pushButton_Update_startec_clicked()
{
    DisabilitaComandi();
    ui->pushButton_Update_startec->setStyleSheet(BOTTONE_GIALLO);

    hHTTPAsync.IndiceMacchinaComando=IndiceMacchinaAttiva;
    TipoComandoAsync=STARTEC;
    ListaECU.at(IndiceMacchinaAttiva)->StatusCommand=QString("0");
    InviaComandoHTTP_Async(UPLOAD_COMANDO_MACCHINA,IndiceMacchinaAttiva);
}

void MainWindow::DisabilitaComandi(void)
{
    ui->pushButton_Stop->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Stop->setEnabled(false);
    ui->pushButton_Timer->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Timer->setEnabled(false);
    ui->pushButton_Update_startec->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Update_startec->setEnabled(false);
    emit DisableTimerQML();
}

void MainWindow::AbilitaComandi(void)
{
    ui->pushButton_Stop->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Stop->setEnabled(true);
    ui->pushButton_Timer->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Timer->setEnabled(true);
    ui->pushButton_Update_startec->setStyleSheet("QPushButton:disabled {background-color: none; color:none}");
    ui->pushButton_Update_startec->setEnabled(true);
    emit EnableTimerQML();
}

//void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent)
//{
//qDebug()<<"APPLEBY: Key Released:" << keyEvent->key();
//    if (keyEvent->key() == Qt::Key_Back){
//        this->close();
//    }
//}//keyReleaseEvent

//void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
//{
//qDebug()<<"APPLEBY: Key Pressed:" << keyEvent->key();
//}//keyPressEvent

//bool MainWindow::eventFilter(QObject *obj,
//                             QEvent *event)
//{
//    QKeyEvent *keyEvent = NULL;//event data, if this is a keystroke event
//    bool result = false;//return true to consume the keystroke

//    if (event->type() == QEvent::KeyPress)
//    {
//         keyEvent = dynamic_cast<QKeyEvent*>(event);
//         this->keyPressEvent(keyEvent);
//         result = true;
//    }//if type()

//    else if (event->type() == QEvent::KeyRelease)
//    {
//        keyEvent = dynamic_cast<QKeyEvent*>(event);
//        this->keyReleaseEvent(keyEvent);
//        result = true;
//    }//else if type()

//    //### Standard event processing ###
//    else
//        result = QObject::eventFilter(obj, event);

//    return result;
//}//eventFilter

void MainWindow::on_pushButton_2_clicked()
{
    on_pushButton_Mappa_clicked();
}

void MainWindow::onBackPressed(){
    emit ui->pushButton_Elenco->click();
    ui->pushButton_Elenco->setFocus();
    this->update();
    QApplication::processEvents();
    this->close();
}

QDir MainWindow::GetProjectPath(void){
    return ProjectPath;
}

void MainWindow::DestroyMap(void){

//    //set layout programatically
// //   ui->frame_Mappa->layout()->removeWidget(Mappa);

//    //distruggi il widget
////    if (Mappa!=nullptr){
////        qDebug()<<"Distruggo Mappa!!!!!!!!!";
////        delete Mappa;
////        Mappa=nullptr;
////    }
////    qDebug()<<"Distruggo Mappa!!!!!!!!!";
////    Mappa->close();
////    delete Mappa;
////    Mappa=nullptr;


//    qDebug()<<"Raise!!!";
//    this->raise();
//    QApplication::processEvents();
//    qDebug()<<"Repaint!!!";
//    this->repaint();
//    QApplication::processEvents();
//    qDebug()<<"Update!!!";
//    this->update();
//    QApplication::processEvents();
//    qDebug()<<"Go To Main!!!";
//    qDebug()<<"rianimati!!!!!!!!!";

//    //Configura e avvia il timer one shot
//    connect(&BaseTempi,SIGNAL(timeout()),this,SLOT(CallbackBaseTempi()));
//    BaseTempi.setInterval(BASE_TEMPI);
//    BaseTempi.start(); //avvia la base tempi a 1 s
//    ui->pushButton_User->setFocus();

////    ui->pushButton_Elenco->click();
//   // QApplication::processEvents();
}
