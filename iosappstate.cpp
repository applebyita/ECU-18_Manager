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

#include "iosappstate.h"
#if defined Q_OS_IOS
#include <QDebug>
#include "QtIosNotifier.h"

IOSAppState* IOSAppState::m_delegate = nullptr;

IOSAppState::IOSAppState(QObject* parent): QObject(parent)
{

}

IOSAppState::~IOSAppState()
{

}

IOSAppState* IOSAppState::getInstance()
{
    if(nullptr == m_delegate)
    {
        m_delegate = new IOSAppState();
    }
    return m_delegate;
}

void IOSAppState::destroyInstance()
{
    if(m_delegate)
    {
        delete m_delegate;
        m_delegate = nullptr;
    }
}

void IOSAppState::applicationDidBecomeActive()
{
    qDebug()<<Q_FUNC_INFO;
}


void IOSAppState::applicationDidEnterBackGround()
{
    qDebug()<<Q_FUNC_INFO;
//    QtIosNotifier* Notifier=new(QtIosNotifier);
//    Notifier->show(tr("Ciao"),tr("Fabrizio"));
}

void IOSAppState::applicationDidEnterForeGround()
{
    qDebug()<<Q_FUNC_INFO;
}
#endif
