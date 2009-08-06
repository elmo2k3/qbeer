/*
 * Copyright (C) 2009 Bjoern Biesenbach <bjoern@bjoern-b.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef QBEER_H
#define QBEER_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QStringList>
#include <QStringListModel>
#include <QSettings>
#include <QTimer>
#include <QSystemTrayIcon>

#include "table_model_users.h"
#include "beer_connection.h"
#include "configdialog.h"

namespace Ui
{
    class qbeer;
}

class qbeer : public QMainWindow
{
    Q_OBJECT

    public:
        qbeer(QWidget *parent = 0);
        ~qbeer();

    private slots:
        void showProperties();
        void connectToHost();
        void gotAuth(QString level); 
        void gotLastTag(QString lastTag, QString lastTime, QString lastUserId);
        void gotConnection(void); 
        void insertEmptyUser(void);
        void insertEmptyUserWithTag(void);
        void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
        void deleteCurrentUser(void);
        void about(void);

    private:
        void readSettings(void);
        void closeEvent(QCloseEvent *event);
        void writeSettings(void);
        Ui::qbeer *ui;
        BeerConnection *connection;
        TableModelUsers *users;
        QTimer *timer;
        QSystemTrayIcon *icon;
        QMenu *iconMenu;
};

#endif // QBEER_H
