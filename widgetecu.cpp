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

#include "widgetecu.h"
#include "ui_widgetecu.h"
#include <QPixmap>
#include <QLabel>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

WidgetECU::WidgetECU(QWidget *parent, ECU_t* pElemento) :
    QWidget(parent),
    ui(new Ui::WidgetECU)
{
    ui->setupUi(this);

//    ui->label_Campi->setVisible(false);
    ui->label_Tipo->setVisible(false);
    ui->label_Status->setVisible(false);

    Elemento=pElemento;

    Connected=true;

    //riscala l'altezza in modo da vedere le scritte
    int width=this->width();
    //height=qApp->fontMetrics().height()*4;

    FontECU=this->font();
#ifdef Q_OS_IOS
    FontECU.setPixelSize((int)((qApp->screens().at(0)->geometry().width()/100))*5.5);
#else
    FontECU.setPixelSize((int)((qApp->screens().at(0)->geometry().width()/100)*4));
#endif
    FontECU.setBold(true);
    this->setFont(FontECU);
    height=this->fontMetrics().height()*4;


    this->setGeometry(0,0,width,height);

    //Icona della macchina
    QString TipoMacchina=Elemento->Descrizione.section(' ', 0, 0, QString::SectionSkipEmpty);

    //Preaparo il graphic view per la macchina
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView_tipo->setScene(scene);

    //Preaparo il graphic view per lo stato
    QGraphicsScene* scene2 = new QGraphicsScene();
    ui->graphicsView_Status->setScene(scene2);

    if (TipoMacchina==tr("GMP")){
         QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/GMP.png")).scaledToWidth((height)-7));
         scene->addItem(item);
    }else{
         QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/PLSMP.png")).scaledToWidth((height)-7));
         scene->addItem(item);
    }

    //ridimensiona la label in modo che sia quadrata
    ui->label_Status->setGeometry(0,0,height,height);

    //Compila il plain intermedio
    ui->plainTextEdit_Campi->setFont(FontECU);
    //compila il campo descrizione: nome della macchina, descrizione, numero di serie
//    ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->Descrizione+tr("\r\nS.N.: ")+Elemento->NumeroSerialeSW);
    ui->plainTextEdit_Campi->setPlainText(Elemento->Nome+tr("\r\n")+Elemento->Descrizione+tr("\r\nS.N.: ")+Elemento->NumeroSerialeSW);

    //Icona di stato
    if (Elemento->Status==ON_LINE){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDVerde.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }else if (Elemento->Status & OFF_LINE){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDGrigio.png")).scaled(height-5,height-4));
        scene2->addItem(item);
        ui->label_Tipo->setEnabled(false);
        ui->plainTextEdit_Campi->setEnabled(false);
        Connected=false;
    }else if (Elemento->Status & MACHINE_WARNING){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDGiallo.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }else {
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDRosso.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }

}

WidgetECU::~WidgetECU()
{
    delete ui;
}

void WidgetECU::Refresh(){
    Connected=true;

    //Icona della macchina
    QString TipoMacchina=Elemento->Descrizione.section(' ', 0, 0, QString::SectionSkipEmpty);

    ui->graphicsView_tipo->scene()->clear();

    //Preaparo il graphic view per la macchina
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView_tipo->setScene(scene);

    ui->graphicsView_Status->scene()->clear();

    //Preaparo il graphic view per lo stato
    QGraphicsScene* scene2 = new QGraphicsScene();
    ui->graphicsView_Status->setScene(scene2);

    if (TipoMacchina==tr("GMP")){
         QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/GMP.png")).scaledToWidth((height)-7));
         scene->addItem(item);
    }else{
         QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/PLSMP.png")).scaledToWidth((height)-7));
         scene->addItem(item);
    }

    //ridimensiona la label in modo che sia quadrata
    ui->label_Status->setGeometry(0,0,height,height);

    //Compila il plain intermedio
    ui->plainTextEdit_Campi->setFont(FontECU);
    //compila il campo descrizione: nome della macchina, descrizione, numero di serie
//    ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->Descrizione+tr("\r\nS.N.: ")+Elemento->NumeroSerialeSW);
    ui->plainTextEdit_Campi->setPlainText(Elemento->Nome+tr("\r\n")+Elemento->Descrizione+tr("\r\nS.N.: ")+Elemento->NumeroSerialeSW);

    //Icona di stato
    if (Elemento->Status==ON_LINE){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDVerde.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }else if (Elemento->Status & OFF_LINE){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDGrigio.png")).scaled(height-5,height-4));
        scene2->addItem(item);
        ui->label_Tipo->setEnabled(false);
        ui->plainTextEdit_Campi->setEnabled(false);
        Connected=false;
    }else if (Elemento->Status & MACHINE_WARNING){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDGiallo.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }else {
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(tr(":/Resources/LEDRosso.png")).scaled(height-5,height-4));
        scene2->addItem(item);
    }
}

