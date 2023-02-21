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

#ifndef IOSAPPSTATE_H
#define IOSAPPSTATE_H
#include <QObject>

#if defined Q_OS_IOS
void InitializeDelegate();

class IOSAppState : QObject
{
    Q_OBJECT
    explicit IOSAppState(QObject* parent=0);
    ~IOSAppState();
public:
    static IOSAppState* getInstance();
    void applicationDidEnterBackGround();
    void applicationDidEnterForeGround();
    void applicationDidBecomeActive();
    static void destroyInstance();
private:
    static IOSAppState* m_delegate;
};

#endif
#endif // IOSAPPSTATE_H

