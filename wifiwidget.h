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

#ifndef WIFIWIDGET_H
#define WIFIWIDGET_H

#include <QWidget>
//#include <QNetworkAccessManager>
//#include <QNetworkConfigurationManager>
#include <QTextEdit>
#include <QStringListModel>
#include <QStackedWidget>

namespace Ui {
class WiFiWidget;
}

typedef struct{
int XStart;
int XEnd;
}Swipe_t;

class WiFiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WiFiWidget(QWidget *parent = 0);
    ~WiFiWidget();

public slots:
    void ConnectECU();

protected:
      bool event(QEvent *event) override;

private slots:
      bool TryConnect();

      void on_Retry_pushButton_clicked();

private:
    Ui::WiFiWidget *ui;
    Swipe_t SwipeData;

//    QNetworkConfigurationManager NetworkManager;
//    QList<QNetworkConfiguration> netcfgList;
    QStringList                  WiFisList;
    QStringListModel             WiFisModel;

    void PaginaAvanti();
    void PaginaIndietro();
//    void UpdateLog(QTextEdit *Edit);
#ifdef Q_OS_ANDROID
//    int fibonacci(int n);
#endif
    void SetStackedWidget(QString Name, QStackedWidget *StackedWidget);
};

#endif // WIFIWIDGET_H
