/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

package org.qtproject.ecu18_manager.notification;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.app.Activity;

import android.media.RingtoneManager;
import android.net.Uri;
import android.graphics.Color;

//Debug
import android.util.Log;


//API26
import android.app.NotificationChannel;


public class NotificationClient extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static Notification.Builder m_builder;
    private static NotificationClient m_instance;
    private static RingtoneManager m_ringtoneManager;
    //Define sound URI
    private static Uri soundUri = m_ringtoneManager.getDefaultUri(m_ringtoneManager.TYPE_NOTIFICATION);
    private static Color m_color;
    private static NotificationManager m_notificationManager=null;

    //API26
//    static NotificationChannel m_notificationChannel;


    public NotificationClient()
    {
        m_instance = this;
    }


    public static void notify(String s, int Id)
    {
        if (android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.O){
        Log.d("APPLEBY:", "Android version>=OREO");
        //API26
        String channelId = "ECU18_id";


        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);

            CharSequence channelName = "ECU18_Channel";
            int importance = m_notificationManager.IMPORTANCE_HIGH;

            NotificationChannel  m_notificationChannel = new NotificationChannel(channelId, channelName, importance);
            m_notificationChannel.enableLights(true);
            m_notificationChannel.setLightColor(Color.BLUE);
            m_notificationChannel.enableVibration(true);
            m_notificationChannel.setVibrationPattern(new long[]{100, 200, 300, 400, 500, 400, 300, 200, 400});

            m_notificationManager.createNotificationChannel(m_notificationChannel);
        }

        if (m_builder == null) {
            m_builder = new Notification.Builder(m_instance);
        }

        m_builder.setContentTitle(s);
        m_builder.setSound(soundUri);
        m_builder.setChannelId(channelId);
        }else{
        Log.d("APPLEBY:", "Android version<OREO");
        //API19
           if (m_notificationManager == null) {
                m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            }
            if (m_builder == null) {
                m_builder = new Notification.Builder(m_instance);
            }
            m_builder.setContentTitle(s);
            m_builder.setContentText("ECU-18 Manager");
            m_builder.setSound(soundUri);
            m_builder.setLights(m_color.BLUE,500,1000);
        }

        m_builder.setSmallIcon(R.drawable.iconalarm);    
        m_notificationManager.notify(Id, m_builder.build());
    }

    public static void notifyWarning(String s, int Id)
    {
        if (android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.O){
     //   NotificationChannel m_notificationChannel=null;
        Log.d("APPLEBY:", "Android version>=OREO");
        //API26
         String channelId = "ECU18_id";

        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            CharSequence channelName = "ECU18_Channel";
            int importance = m_notificationManager.IMPORTANCE_HIGH;
            NotificationChannel m_notificationChannel = new NotificationChannel(channelId, channelName, importance);
            m_notificationChannel.enableLights(true);
            m_notificationChannel.setLightColor(Color.BLUE);
            m_notificationChannel.enableVibration(true);
            m_notificationChannel.setVibrationPattern(new long[]{100, 200, 300, 400, 500, 400, 300, 200, 400});
            m_notificationManager.createNotificationChannel(m_notificationChannel);
        }

        if (m_builder == null) {
            m_builder = new Notification.Builder(m_instance);
        }

        m_builder.setContentTitle(s);
        m_builder.setSound(soundUri);
        m_builder.setChannelId(channelId);
        }else{
        Log.d("APPLEBY:", "Android version<OREO");
        //API19
           if (m_notificationManager == null) {
                m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            }

        }

        if (m_builder == null) {
        m_builder = new Notification.Builder(m_instance);
        }

        m_builder.setContentTitle(s);
        m_builder.setContentText("ECU-18 Manager");
        m_builder.setSound(soundUri);
        m_builder.setLights(m_color.BLUE,500,1000);
        m_builder.setSmallIcon(R.drawable.iconwarning);
        m_notificationManager.notify(Id, m_builder.build());
    }

    public static void MoveAppInBackground(){
            Log.d("APPLEBY:", "Move java to back");
            m_instance.moveTaskToBack(true);
    }
}
