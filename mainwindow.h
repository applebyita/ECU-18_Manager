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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QUrl>
#include "notificationclient.h"
#include "ecu.h"
#include <QListWidget>
#include <QTimer>
#include <QDir>
#include <QQmlContext>
#include "mapwidget.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

#define SECONDI_OFFLINE         30*(60) // in sec
#define BASE_TEMPI              1000    // in ms
#define SECONDI_REFRESH_STATO   10      //in sec
#define SECONDI_REFRESH_COMANDO 10      //in sec
#define TIMEOUT_CONNESSIONE     15      //in sec

#define BOTTONE_GIALLO "QPushButton {background-color: rgb(255, 255, 127); color: rgb(255, 255, 127);} QPushButton:hover {background-color: rgb(255, 255, 127); color: rgb(255, 255, 127);}"
#define BOTTONE_VERDE "QPushButton {background-color: green; color: green;} QPushButton:hover {background-color: green;  color: green;}"
#define BOTTONE_ROSSO "QPushButton {background-color: red; color: red;} QPushButton:hover {background-color: red;  color: red;}"


typedef enum{
NESSUN_COMANDO=0,
DOWNLOAD_STATO_MACCHINA=2,
DOWNLOAD_ELENCO_MACCHINE=11,
DOWNLOAD_DETTAGLI_MACCHINE=12,
UPLOAD_COMANDO_MACCHINA=13,
DOWNLOAD_COMANDO_MACCHINA=15,
}Comandi_Server_t;

typedef enum{
NO_COMMAND,
STOP,
TIMER,
STARTEC
}TipoComando_t;

typedef enum{
HTTP_NONE,
HTTP_OK,
HTTP_ERROR,
}HTTP_Result_t;

typedef struct{
Comandi_Server_t        ComandoAttuale;
int                     IndiceMacchinaComando;
HTTP_Result_t           Result;
quint8                  Retry;
}HTTP_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
#ifdef Q_OS_ANDROID
    NotificationClient* notificationClient;
#endif

    //variabili uso comando timer
    Q_PROPERTY(int timerore MEMBER TimerOre NOTIFY TimerOreChanged)
    Q_PROPERTY(int timerminuti MEMBER TimerMinuti NOTIFY TimerMinutiChanged)

    //variabili uso mappa
//    Q_PROPERTY(double latitude MEMBER Latitude NOTIFY LatitudeChanged)
//    Q_PROPERTY(double longitude MEMBER Longitude NOTIFY LongitudeChanged)
//    Q_PROPERTY(QString fileHTML MEMBER FileHTML NOTIFY FileHTMLChanged)
//    Q_PROPERTY(QString fileHTML_Blank MEMBER FileHTML_Blank NOTIFY FileHTMLChanged)

    QDir GetProjectPath(void);
    void RichiediElencoMacchine(void);

    HTTP_t                hHTTP;


signals:
    void TimerOreChanged();
    void TimerMinutiChanged();
    void DisableTimerQML();
    void EnableTimerQML();
//    void LatitudeChanged();
//    void LongitudeChanged();


    void updateTumblerVel_Imp_startec(QVariant Centinaia, QVariant Decine, QVariant Unita);
    void updateTumblerPausa_Start_startec(QVariant Centinaia, QVariant Decine, QVariant Unita);
    void updateTumblerPausa_End_startec(QVariant Centinaia, QVariant Decine, QVariant Unita);
    void updateTumblerVel_Imp_startec_bianco(void);
    void updateTumblerVel_Imp_startec_giallo(void);
    void updateTumblerPausa_Start_startec_bianco(void);
    void updateTumblerPausa_Start_startec_giallo(void);
    void updateTumblerPausa_End_startec_bianco(void);
    void updateTumblerPausa_End_startec_giallo(void);


public slots:
    void stateChanged(Qt::ApplicationState newState);
    void VelImpStartec_Item_cifra_changed(int Position, int Value); 
    void PausaStartStartec_Item_cifra_changed(int Position, int Value);
    void PausaEndStartec_Item_cifra_changed(int Position, int Value);
    void onBackPressed();
    void DestroyMap(void);

private slots:
    void on_pushButton_Connect_clicked();

    void IncomingData();
 //   void on_pushButton_Send_clicked();

    void httpReadyRead();
    void httpFinished();


    void on_pushButton_clicked();
    void on_pushButton_Team_clicked();

    void on_pushButton_Elenco_clicked();

    void on_pushButton_Motore_clicked();

    void on_pushButton_Startec_clicked();

    void on_pushButton_Mappa_clicked();

    void on_pushButton_Wifi_clicked();

    void on_listWidget_ECU_itemClicked(QListWidgetItem *item);

    void on_pushButton_Riprova_clicked();

    void CallbackBaseTempi();

    void InviaComandoHTTP(Comandi_Server_t ComandoAttuale, int IndiceMacchina);

    void InviaComandoHTTP_Async(Comandi_Server_t  ComandoAttuale,int IndiceMacchina);

    void on_pushButton_User_clicked();

    void on_pushButton_Login_clicked();

    void on_pushButton_Stop_clicked();

    void httpFinishedAsync();
    void httpReadyReadAsync();
    void on_pushButton_Timer_clicked();

//    void on_lineEdit_Timer_Ore_returnPressed();
//    void on_lineEdit_Timer_Minuti_returnPressed();
//    void Callback_Timer_Ore_SelectAll();
//    void Callback_Timer_Minuti_SelectAll();

//    void RicaricaMappa();

    void on_stackedWidget_App_currentChanged(int arg1);

//    void RischiacciaBottoneMappa(void);

    void on_pushButton_Update_startec_clicked();


    void on_pushButton_2_clicked();

protected:
    void changeEvent(QEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

//    bool eventFilter(QObject *object, QEvent *event) override;
//    void keyPressEvent(QKeyEvent *keyEvent) override;
//    void keyReleaseEvent(QKeyEvent *keyEvent) override;

private:
    Ui::MainWindow *ui;

    //proprietà per gestione server php
    QNetworkAccessManager qnam;

    QUrl                  url;
    QNetworkReply*        reply;
    bool                  httpRequestAborted;
 //   HTTP_t                hHTTP;
    uint32_t              TimerTimeoutConnessione=0;

    QUrl                  urlAsync;
    QNetworkReply*        replyAsync;
    bool                  httpRequestAbortedAsync;
    HTTP_t                hHTTPAsync;
    uint32_t              TimerTimeoutConnessioneAsync=0;

    //blocco valori da duplicare per le varie macchine
    bool CheckComandoEseguito=false;
//    int  IndiceMacchinaComandoAsync=0;
    TipoComando_t TipoComandoAsync=NO_COMMAND;

    char ContatoreResize;

    QList<ECU_t*> ListaECU;

    int         IndiceMacchinaAttiva=-1;

    int         PrescalerStato=0;
    int         PrescalerComando=0;

    QTimer      BaseTempi;

    //variabili uso ini
    QDir            ProjectPath;

    //variabili login
    QString         UserName;
    QString         Password;


    QString StringaUltimoComando;

    bool InitOk=false;

    int NotificationID=0;
    bool ComandiUpdated=false;

    QTimer TimerLineditOre;
    QTimer TimerLineditMinuti;
    QQmlContext* ContextTimer;

    int   TimerOre=0;
    int   TimerMinuti=0;

//    QQmlContext* ContextMappa;

    double Latitude=0;
    double Longitude=0;
    MapWidget*  Mappa=nullptr;

//    QTimer TimerRicaricaMappa;
//    QTimer TimerRicaricaMappa1;

    QQmlContext* Context_Vel_Imp_startec;
    QQmlContext* Context_Pausa_Start_startec;
    QQmlContext* Context_Pausa_End_startec;

    int          Vel_Imp_startec;
    int          Pausa_Start_startec;
    int          Pausa_End_startec;


//     QString FileHTML_Blank="\n"
//        "<!DOCTYPE html>\n"
//        "<html>\n"
//        "<head>\n"
//        "<style>\n"
//        "html, body, #gmap_canvas {\n"
//        "    width: 100%;\n"
//        "    height: 100%;\n"
//        "    margin: 0;\n"
//        "    padding: 0;\n"
//        "}\n"
//        "#map_canvas {\n"
//        "    position: relative;\n"
//        "}\n"
//        "</style>\n"
//        "</head>\n"

//        "<body>\n"
//        "Loading..."
//        "</body>\n"

//        "</html>";


 //   char RicaricaMappaAndroid=0;

    QStringList StatiStartec={
    "INIZIALIZZAZIONE",
    "",
    "",
    "ATTESA RIPARTENZA",
    "ATTESA RIAVVIO",
    "ATTESA SPEGNIMENTO",
    "SROTOLAMENTO",
    "",
    "",
    "PAUSA INIZIALE",
    "",
    "REGOLAZIONE",
    "",
    "PAUSA FINALE",
    "",
    "",
    "FINE IRRIGAZIONE"};

    QStringList EventiStartec={
    "STATO",
    "FINE",
    "ERRORE FINE",
    "ALM VELOCITA' ZERO",
    "ALM PRESSIONE ZERO",
    "ERR REGOLAZIONE",
    "ERRORE VELOCITA'",
    "",
    "FINE RIAVVOLGIMENTO"
    "START",
    "ERRORE START",
    "STOP",
    "ERRORE STOP",
    "",
    ""
    "AVV BATTERIA BASSA",
    "ERR BATTERIA",
    "ERR SOVRACORR. MOTORE FLUSSO",
    "ERR TIMEOUT MOTORE FLUSSO",
    "ERR SOVRACORRENTE MOTORE REGOLAZIONE",
    "ERR TIMEOUT MOTORE REGOLAZIONE",
    "RESTART RAIN260",
    "ERR ARRESTO CRITICO RAIN260",
    "ERR INTERF MOTOPOMPA",
    "GATEWAY MOTOPOMPA",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "ERR VENTO SOPRA SOGLIA STOP",
    "ERR VENTO SOPRA SOGLIA SOSP",
    "VENTO SOTTO SOGLIA REINIZIO",
    "AVV VENTO SOPRA SOGLIA",
    "AVV VENTO SOTTO SOGLIA",
    "ERR PRESSIONE SOPRA SOGLIA STOP",
    "ERR PRESSIONE SOTTO SOGLIA STOP",
    "ERR PRESSIONE SOTTO SOGLIA SOSP",
    "PRESSIONE SOPRA SOGLIA REIINIZIO",
    "AVV PRESSIONE SOPRA SOGLIA",
    "AVV PRESSIONE SOTTO SOGLIA",
    "",
    "",
    "AVV INGRESSO 1",
    "AVV INGRESSO 2"
    };

    //variabili uso widget
    void ShowTerminalMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalErrorMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalAssertMessage(QTextEdit* Edit, QString Message);
    void startRequest(const QUrl &requestedUrl);
    void startRequest_Async(const QUrl &requestedUrl);
    void Refresh_Lista_ECU();
    void InterpreteRispostaHttp(QString replyTemp);
//    void RichiediElencoMacchine();
    void InterpreteRispostaHttpAsync(QString replyTemp);
    void UpdateTastiTimerStop();
    void CaricaMappa();
    void DisabilitaComandi();
    void AbilitaComandi();
    void activate_page_Startec();
    void activate_page_Allarmi_Startec();
};

#endif // MAINWINDOW_H
