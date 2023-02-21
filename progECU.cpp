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

#include "progECU.h"
#include "ui_progECU.h"
#include <QtNetwork>
#include <QFileDialog>
#include <QTimer>

#define BLOCK_SIZE      512

progECU::progECU(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progECU)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    ui->textEdit->setCursorWidth(0);

//    this->setWindowFlags(Qt::WindowCloseButtonHint); //Esempio per attivare i bottoni della finestra

    connect(&tcpClient, SIGNAL(readyRead()),this, SLOT(IncomingData()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(StatoMacchina()));

    ui->lineEdit_IP->setText("192.168.4.1");
    ui->lineEdit_Port->setText("333");
    ui->pushButton_Send->setEnabled(false);
    ui->tabWidget->setTabEnabled(1,false);
}

progECU::~progECU()
{
    delete ui;
}

void progECU::IncomingData(void){
//    ShowTerminalMessage(ui->textEdit, QString::fromStdString(tcpClient.readAll().toStdString()));

    DataRX=tcpClient.readAll();
    // UPDATE
    if(ui->tabWidget->currentIndex()==0){
        if((DataRX[0]=='O')&&(DataRX[1]=='K')) OkRic=1;
        else if((DataRX[0]=='E')&&(DataRX[1]=='E')) OkRic=2;

        QTimer::singleShot(200,this,SLOT(on_pushButton_Send_clicked()));
    }
    // STATUS
    else if(ui->tabWidget->currentIndex()==1){
        if((DataRX[0]=='$')&&(DataRX[1]=='S')&&(DataRX[2]=='T')) OkRic=3;
        else if((DataRX[0]=='E')&&(DataRX[1]=='E')) OkRic=2;

        QTimer::singleShot(500,this,SLOT(StatoMacchina()));
    }

}


//------------------------- PULSANTI -----------------------------------------------------------------------------------

void progECU::on_pushButton_File_clicked()
{
    PercorsoBIN=QFileDialog::getOpenFileName(this,tr("Open File"),QStandardPaths::writableLocation((QStandardPaths::DocumentsLocation)),tr("Binay (*.bin)"));
    ui->lineEdit_File->setText(PercorsoBIN);
}

void progECU::on_pushButton_Connect_clicked()
{
    ShowTerminalMessage(ui->textEdit,QString("Connecting to %1:%2 Server").arg(ui->lineEdit_IP->text()).arg(ui->lineEdit_Port->text().toInt()));
    tcpClient.connectToHost(ui->lineEdit_IP->text(), ui->lineEdit_Port->text().toInt());

    if(tcpClient.waitForConnected(3000)){
        ShowTerminalAssertMessage(ui->textEdit,"Connected!");
        ui->pushButton_Send->setEnabled(true);
        ui->tabWidget->setTabEnabled(1,true);
        ui->tabWidget->setCurrentIndex(1);
        ConnectedECU=true;
    }else{
       ShowTerminalErrorMessage(ui->textEdit,"Not Connected!");
       ConnectedECU=false;
    }
}

void progECU::on_pushButton_Send_clicked()
{
    qint16 i=0;
    unsigned char CheckSum=0;
    QByteArray DataTX;

    // Apertura file BIN e calcolo numero blocchi da trasmettere
    if(StepTX==0){
        if(ui->lineEdit_File->text().isEmpty()){
            ShowTerminalErrorMessage(ui->textEdit, "NO FILE SELECTED!");
            return;
        }
        else {
            FileBIN.setFileName(PercorsoBIN);
            ncicli = FileBIN.size()/BLOCK_SIZE;
            mcicli = FileBIN.size()%BLOCK_SIZE;
            if(mcicli!=0) ncicli=ncicli+1;
            FileBIN.open(QFile::ReadOnly);
            StepTX=1; CRC_bin=0;
        }
    }

    // Comando ERASE
    if(StepTX==1){
        ShowTerminalMessage(ui->textEdit, "Erasing...");
        OkRic=0;
        DataTX[0]='$'; DataTX[1]='E'; DataTX[2]='R'; DataTX[3]='x';
        for(i=0; i<4; i++) CheckSum=CheckSum^DataTX[i];
        DataTX[4]=CheckSum;
        tcpClient.write(DataTX,5);
        StepTX=2;
    }
    if(StepTX==2){
        if(OkRic==1){
            ShowTerminalMessage(ui->textEdit, "ERASE DONE");
            OkRic=0; StepTX=3;
        }
        else if(OkRic==2){
            ShowTerminalErrorMessage(ui->textEdit, "ERASE ERROR");
            FileBIN.close();
        }
    }
    // Comando WRITE
    if(StepTX==3){
 SendData:
        OkRic=0; CheckSum=0;
        DataTX[0]='$'; DataTX[1]='W'; DataTX[2]='R';
        // Address
        DataTX[3]=(unsigned char)(Address>>24);
        DataTX[4]=(unsigned char)(Address>>16);
        DataTX[5]=(unsigned char)(Address>>8);
        DataTX[6]=(unsigned char)Address;
        for(i=7; i<(BLOCK_SIZE+7); i++) DataTX[i]=(unsigned char)0xFF;    // inizializzazione buffer
        if(nn==ncicli){
            if(mcicli) N_ByteF=mcicli;
            else N_ByteF=BLOCK_SIZE;
        }
        else N_ByteF=BLOCK_SIZE;
        DataTX.insert(7,FileBIN.read(N_ByteF));
        CRC_bin = CRC_Update(CRC_bin,DataTX.mid(7,BLOCK_SIZE),BLOCK_SIZE);
        for(i=0; i<(BLOCK_SIZE+7); i++) CheckSum=CheckSum^DataTX[i];
        DataTX[BLOCK_SIZE+7]=CheckSum;
        tcpClient.write(DataTX,BLOCK_SIZE+8);
        StepTX=4;
    }
    if(StepTX==4){
        if(OkRic==1){
            ShowTerminalMessage_INS(ui->textEdit, tr("Upload %1 percent ...").arg((quint16)(((float)nn/ncicli)*100)));
            OkRic=0; nn++; Address+=BLOCK_SIZE;
            if(nn<=ncicli) goto SendData; else StepTX=5;
        }
        else if(OkRic==2){
            ShowTerminalErrorMessage(ui->textEdit, "WRITE ERROR");
            FileBIN.close();
        }
    }
    if(StepTX==5){
        ShowTerminalAssertMessage(ui->textEdit, "UPLOAD COMPLETE");
        FileBIN.close();
        StepTX=6;
    }
    // Comando CHECK CRC
    if(StepTX==6){
        ShowTerminalMessage(ui->textEdit, "CRC verify...");
        OkRic=0;
        DataTX[0]='$'; DataTX[1]='C'; DataTX[2]='K';
        DataTX[3]= (unsigned char)(CRC_bin>>8);
        DataTX[4]= (unsigned char)(CRC_bin);
        for(i=0; i<5; i++) CheckSum=CheckSum^DataTX[i];
        DataTX[5]=CheckSum;
        tcpClient.write(DataTX,6);
        StepTX=7;
    }
    if(StepTX==7){
        if(OkRic==1){
            ShowTerminalAssertMessage(ui->textEdit, "CRC OK");
            OkRic=0;
        }
        else if(OkRic==2){
            ShowTerminalErrorMessage(ui->textEdit, "CRC ERROR");
        }
    }

}

void progECU::on_pushButton_Inc_pressed()
{
    Pulsanti[0]=1;
    ui->pushButton_Inc->setStyleSheet("font: bold 22pt; background-color: rgb(255, 0, 0)");
}

void progECU::on_pushButton_Inc_released()
{
    Pulsanti[0]=0;
    ui->pushButton_Inc->setStyleSheet("font: bold 22pt");
}

void progECU::on_pushButton_Dec_pressed()
{
    Pulsanti[1]=1;
    ui->pushButton_Dec->setStyleSheet("font: bold 22pt; background-color: rgb(255, 0, 0)");
}

void progECU::on_pushButton_Dec_released()
{
    Pulsanti[1]=0;
    ui->pushButton_Dec->setStyleSheet("font: bold 22pt");
}

void progECU::on_pushButton_Escl_clicked()
{
    if(Pulsanti[2]){
        Pulsanti[2]=0;
        ui->pushButton_Escl->setStyleSheet("font: bold 22pt");
    }
    else {
        Pulsanti[2]=1;
        ui->pushButton_Escl->setStyleSheet("font: bold 22pt; background-color: rgb(255, 0, 0)");
    }
}

void progECU::on_pushButton_Auto_clicked()
{
    if(Pulsanti[3]){
        Pulsanti[3]=0;
        ui->pushButton_Auto->setStyleSheet("font: bold 22pt");
    }
    else {
        Pulsanti[3]=1;
        ui->pushButton_Auto->setStyleSheet("font: bold 22pt; background-color: rgb(255, 0, 0)");
    }
}

void progECU::on_pushButton_Fari_clicked()
{
    if(Pulsanti[4]){
        Pulsanti[4]=0;
        ui->pushButton_Fari->setStyleSheet("font: bold 22pt");
    }
    else {
        Pulsanti[4]=1;
        ui->pushButton_Fari->setStyleSheet("font: bold 22pt; background-color: rgb(255, 0, 0)");
    }
}

//------------------------- MESSAGGI ------------------------------------------------------------------------------------

void progECU::ShowTerminalMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::white);
    //Output to log window
    Edit->append(Message);
}

void progECU::ShowTerminalMessage_INS(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::white);
    //Output to log window
    Edit->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
    Edit->moveCursor( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
    Edit->moveCursor( QTextCursor::End, QTextCursor::KeepAnchor );
    Edit->textCursor().removeSelectedText();
    Edit->setTextColor(Qt::white);
    Edit->insertPlainText(Message);
}

void progECU::ShowTerminalErrorMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::red);
    //Output to log window
    Edit->append(Message);
}

void progECU::ShowTerminalAssertMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::green);
    //Output to log window
    Edit->append(Message);
}

//------------------------- FUNZIONI ------------------------------------------------------------------------------------

// Richiesta stato alla centralina e invio stato pulsanti
void progECU::StatoMacchina(void){

    qint16 i=0;
    unsigned char CheckSum=0;
    QByteArray DataTX;

    // Se selezionato il tab di visualizzazione stato centralina
    if(ui->tabWidget->currentIndex()==1){
        // Comando CM per richiesta stato e invio comandi
        if(StepCM==0){
SendCM:
            OkRic=0; CheckSum=0;
            DataTX[0]='$'; DataTX[1]='C'; DataTX[2]='M';
            DataTX[3]=Pulsanti[0];  // +
            DataTX[4]=Pulsanti[1];  // -
            DataTX[5]=Pulsanti[2];  // Escl. Pompa
            DataTX[6]=Pulsanti[3];  // Auto
            DataTX[7]=Pulsanti[4];  // Fari
            DataTX[8]=0;
            for(i=0; i<9; i++) CheckSum=CheckSum^DataTX[i];
            DataTX[9]=CheckSum;
            tcpClient.write(DataTX,10);
            StepCM=1;
        }
        if(StepCM==1){
            if(OkRic==3){
                CheckSum=0;
                for(i=0; i<15; i++) CheckSum=CheckSum^DataRX[i];
                if(CheckSum==((unsigned char)DataRX[15])){
                    GiriMotore = ((quint16)DataRX[3])*256+DataRX[4];
                    Pressione = (((float)((unsigned char)DataRX[5])))/10;
                    Depressione = ((((float)((unsigned char)DataRX[6])))/10)-1;
                    PressRec = DataRX[7];
                    PressRecImp = DataRX[8];
                    Gasolio = DataRX[9];
                    TempH2O = DataRX[10];
                    Vbat = (((float)((unsigned char)DataRX[11])))/5;

                    if(((quint8)DataRX[12])!=Pulsanti[2]){
                        if(SaltoRit1==0) SaltoRit1++; else {SaltoRit1=0; on_pushButton_Escl_clicked();}
                    }
                    else SaltoRit1=0;
                    if(((quint8)DataRX[13])!=Pulsanti[3]){
                        if(SaltoRit2==0) SaltoRit2++; else {SaltoRit2=0; on_pushButton_Auto_clicked();}
                    }
                    else SaltoRit2=0;
                    if(((quint8)DataRX[14])!=Pulsanti[4]){
                        if(SaltoRit3==0) SaltoRit3++; else {SaltoRit3=0; on_pushButton_Fari_clicked();}
                    }
                    else SaltoRit3=0;

                    //Aggiorna la grafica del pannello locale
                    ui->widgetPannelloECU->SetGiriMotore(GiriMotore);
                    ui->widgetPannelloECU->SetPressioneDepressione(Pressione,Depressione);
                    ui->widgetPannelloECU->SetPressRecPressRecImp(PressRec,PressRecImp);
                    ui->widgetPannelloECU->SetGasolio(Gasolio);
                    ui->widgetPannelloECU->SetTempH2O(TempH2O);
                    ui->widgetPannelloECU->SetVbat(Vbat);
                }
                OkRic=0; StepCM=0; goto SendCM;
            }
            else if(OkRic==2){
                ShowTerminalErrorMessage(ui->textEdit, "ERROR");
            }
        }

    }
}

// Calcolo del CRC di un array di byte partendo già da un CRC calcolato in precedenza
quint16 progECU::CRC_Update(quint16 CRC1, QByteArray buf, quint16 lbuf){

    quint8 jj=0, k=0, CRC1_LSB=0;
    quint16 ii=0;

    for(ii=0; ii<lbuf; ii++){
        CRC1_LSB = (quint8)CRC1;
        CRC1_LSB = CRC1_LSB ^ buf[ii];
        CRC1 = CRC1 & 0xFF00;
        CRC1 = CRC1 + CRC1_LSB;
        for(jj=0; jj<8; jj++){
            k = CRC1 & 1;
            CRC1 = CRC1 >> 1;
            if(k>0) CRC1 = CRC1 ^ 0xA001;
        }
    }
    return CRC1;
}

bool progECU::Connect(){
    if (ConnectedECU==false){
    on_pushButton_Connect_clicked();
    }
return ConnectedECU;
}

bool progECU::isConnected(){
    return ConnectedECU;
}

