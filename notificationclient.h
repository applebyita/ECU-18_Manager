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

#ifndef NOTIFICATIONCLIENT_H
#define NOTIFICATIONCLIENT_H

#include <QObject>
#ifdef Q_OS_ANDROID
class NotificationClient : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)
public:
    explicit NotificationClient(QObject *parent = 0);

    void    setNotification(const QString &notification, int Id);
    void    setNotificationWarning(const QString &notification, int Id);
    void    GoToBackground(void);
    QString notification() const;

signals:
    void notificationChanged();
    void notificationChangedWarning();

private slots:
    void updateAndroidNotification();
    void updateAndroidNotificationWarning();

private:
    QString m_notification;
    int Id_notification;
};
#endif

#endif // NOTIFICATIONCLIENT_H
