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

#include "notificationclient.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QDebug>

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
    connect(this, SIGNAL(notificationChangedWarning()), this, SLOT(updateAndroidNotificationWarning()));
}

void NotificationClient::setNotification(const QString &notification, int Id)
{
    m_notification = notification;
    Id_notification= Id;
    qDebug() << tr("Notification emitted:%1").arg(Id_notification);
    emit notificationChanged();
}

void NotificationClient::setNotificationWarning(const QString &notification, int Id)
{
    m_notification = notification;
    Id_notification= Id;
    qDebug() << tr("Notification Warning emitted:%1").arg(Id_notification);
    emit notificationChangedWarning();
}

QString NotificationClient::notification() const
{
    return m_notification;
}

void NotificationClient::updateAndroidNotification()
{
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
    jint NotificationID=static_cast<jint>(Id_notification);
    qDebug() << tr("Id:%1").arg(Id_notification);
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/ecu18_manager/notification/NotificationClient",
                                       "notify",
                                       "(Ljava/lang/String;I)V",
                                       javaNotification.object<jstring>(),
                                       NotificationID
                                       );
}

void NotificationClient::updateAndroidNotificationWarning()
{
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
    jint NotificationID=static_cast<jint>(Id_notification);
    qDebug() << tr("Id:%1").arg(Id_notification);
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/ecu18_manager/notification/NotificationClient",
                                       "notifyWarning",
                                       "(Ljava/lang/String;I)V",
                                       javaNotification.object<jstring>(),
                                       NotificationID
                                       );
}

void NotificationClient::GoToBackground(void)
{
    qDebug() << tr("Going in background!");
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/ecu18_manager/notification/NotificationClient",
                                               "MoveAppInBackground"
                                              );
}
#endif
