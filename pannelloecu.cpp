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

#include "pannelloecu.h"
#include "ui_pannelloecu.h"
#include <QScreen>

PannelloECU::PannelloECU(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PannelloECU)
{
    ui->setupUi(this);

    //Selezione del font dell'elenco pannello
    //int FontWidth=ui->lineEdit_Press->width()/7;

    int FontHeight=ui->lineEdit_Press->height();

    QFont FontPannello=qApp->font();
        if (qApp->screens().at(0)->geometry().width()>=1080){
            FontHeight=FontHeight*2.5;
        }

#ifdef Q_OS_IOS
    FontHeight=(int)(FontHeight/3.5);
#else
    FontHeight=(int)(FontHeight/2.5);
#endif
    FontPannello.setPixelSize(FontHeight);
    ui->lineEdit_Press->setFont(FontPannello);
    ui->lineEdit_Rec->setFont(FontPannello);
    ui->lineEdit_RPM->setFont(FontPannello);
}

PannelloECU::~PannelloECU()
{
    delete ui;
}

void PannelloECU::SetGiriMotore(uint16_t GiriMotore){
    ui->lineEdit_RPM->setText(QString("%1  RPM").arg(GiriMotore));
}

void PannelloECU::SetPressioneDepressione(float Pressione, float Depressione){
ui->lineEdit_Press->setText(QString("%1  %2  BAR").arg(Pressione, 0, 'f', 1).arg(Depressione, 0, 'f', 1));
}

void PannelloECU::SetPressRecPressRecImp(quint8 PressRec, quint8 PressRecImp){
ui->lineEdit_Rec->setText(QString("%1  %2  BAR").arg(PressRec).arg(PressRecImp));
}

void PannelloECU::SetGasolio(quint8 Gasolio){
ui->progressBar->setValue(Gasolio);
}

void PannelloECU::SetTempH2O(quint8 TempH2O){
ui->label_Temp->setText(QString("%1°").arg(TempH2O));
}

void PannelloECU::SetVbat(float Vbat){
ui->label_VBat->setText(QString("%1V").arg(Vbat, 0, 'f', 1));
}
