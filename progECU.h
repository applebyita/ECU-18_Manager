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

#ifndef PROGECU_H
#define PROGECU_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QByteArray>
#include <QFile>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace Ui {
class progECU;
}

class progECU : public QWidget
{
    Q_OBJECT

public:
    explicit progECU(QWidget *parent = 0);
    ~progECU();

public slots:
    bool Connect();
    bool isConnected();

private slots:
    void IncomingData();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Send_clicked();
    void on_pushButton_File_clicked();
    void StatoMacchina();
    void on_pushButton_Inc_pressed();
    void on_pushButton_Inc_released();
    void on_pushButton_Dec_pressed();
    void on_pushButton_Dec_released();

    void on_pushButton_Escl_clicked();

    void on_pushButton_Auto_clicked();

    void on_pushButton_Fari_clicked();

private:
    Ui::progECU *ui;

    bool ConnectedECU=false;

    //Client TCP
    QTcpSocket tcpClient;

    QByteArray DataRX;

    QString PercorsoBIN;
    QFile FileBIN;
    quint8 OkRic=0, StepTX=0, StepCM=0, SaltoRit1=0, SaltoRit2=0, SaltoRit3=0;
    quint16 N_ByteF=0, nn=1, ncicli=0, mcicli=0, CRC_bin=0;
    quint32 Address=0;
    quint8 Pulsanti[5]={0,0,0,0,0};

    quint16 GiriMotore=0;
    float Pressione=0, Depressione=0, Vbat=0;
    quint8 PressRec=0, PressRecImp=0, Gasolio=0, TempH2O=0;

    void ShowTerminalMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalMessage_INS(QTextEdit* Edit, QString Message);
    void ShowTerminalErrorMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalAssertMessage(QTextEdit* Edit, QString Message);

    quint16 CRC_Update(quint16 CRC1, QByteArray buf, quint16 lbuf);

};

#endif // PROGECU_H
