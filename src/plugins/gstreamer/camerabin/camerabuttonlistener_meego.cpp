/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "camerabuttonlistener_meego.h"

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qwidget.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

CameraButtonListener::CameraButtonListener(QObject *parent) :
    QObject(parent),
    m_focusPressed(false),
    m_shutterPressed(false)
{
    m_keys = new MeeGo::QmKeys(this);
    connect(m_keys, SIGNAL(keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)),
            this, SLOT(handleQmKeyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)));
}

CameraButtonListener::~CameraButtonListener()
{
}

void CameraButtonListener::handleQmKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    if (key == MeeGo::QmKeys::Camera) {
        QWidget *window = QApplication::focusWidget();

        bool focusPressed = (state == MeeGo::QmKeys::KeyHalfDown) ||
                            (state == MeeGo::QmKeys::KeyDown);

        if (m_focusPressed != focusPressed) {
            m_focusPressed = focusPressed;
            if (window) {
                QApplication::postEvent(window,
                                        new QKeyEvent(focusPressed ? QEvent::KeyPress : QEvent::KeyRelease,
                                                      Qt::Key_CameraFocus,
                                                      Qt::NoModifier));
            }
        }

        bool shutterPressed = (state == MeeGo::QmKeys::KeyDown);
        if (m_shutterPressed != shutterPressed) {
            m_shutterPressed = shutterPressed;
            if (window) {
                QApplication::postEvent(window,
                                        new QKeyEvent(shutterPressed ? QEvent::KeyPress : QEvent::KeyRelease,
                                                      Qt::Key_Camera,
                                                      Qt::NoModifier));
            }
        }
    }
}

QT_END_NAMESPACE
