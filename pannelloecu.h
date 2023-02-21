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

#ifndef PANNELLOECU_H
#define PANNELLOECU_H

#include <QWidget>

namespace Ui {
class PannelloECU;
}

class PannelloECU : public QWidget
{
    Q_OBJECT

public:
    explicit PannelloECU(QWidget *parent = 0);
    ~PannelloECU();

public slots:
    void SetGiriMotore(uint16_t GiriMotore);
    void SetPressioneDepressione(float Pressione, float Depressione);
    void SetPressRecPressRecImp(quint8 PressRec, quint8 PressRecImp);
    void SetGasolio(quint8 Gasolio);
    void SetTempH2O(quint8 TempH2O);
    void SetVbat(float Vbat);
private:
    Ui::PannelloECU *ui;
};

#endif // PANNELLOECU_H
