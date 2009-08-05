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

#include <iostream>
#include <QCloseEvent>
#include "qbeer.h"
#include "ui_qbeer.h"

qbeer::qbeer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::qbeer)
{
    QSettings settings;

    QCoreApplication::setOrganizationName("beernary-counter");
    QCoreApplication::setOrganizationDomain("geraffel-village.de");
    QCoreApplication::setApplicationName("qbeer");

    ui->setupUi(this);

    readSettings();

    connection = new BeerConnection(this);
    users = new TableModelUsers(this);
    timer = new QTimer(this);
    iconMenu = new QMenu(this);
    iconMenu->addAction(ui->actionQuit);
    icon = new QSystemTrayIcon(QIcon(":/icons/res/Beer_mug.png"),this);
    icon->setContextMenu(iconMenu);
    icon->show();
    
    connect(connection, SIGNAL(gotAuth(QString)), this, SLOT(gotAuth(QString)));
    connect(connection, SIGNAL(gotLastTag(QString,QString)), this, SLOT(gotLastTag(QString,QString)));
    connect(connection, SIGNAL(gotUser(struct User)), users, SLOT(insertUser(struct User)));
    connect(connection, SIGNAL(connected()), this, SLOT(gotConnection()));
    connect(users, SIGNAL(updateUser(struct User)), connection, SLOT(updateUser(struct User)));
    connect(icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
        SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    
    ui->tableViewTags->setModel(users);
    connectToHost();
}

void qbeer::gotLastTag(QString lastTag, QString lastTime)
{
    ui->lineEditLastTag->setText(lastTag);
    ui->lineEditLastTime->setText(lastTime);
}

void qbeer::connectToHost()
{
    QSettings settings;
    connection->disconnect();
    connection->connectToHost(
        settings.value("address","localhost").toString(),
        settings.value("port",5335).toInt());
}

void qbeer::gotConnection()
{
    QSettings settings;
    connection->auth(settings.value("user").toString(),
        settings.value("password").toString());
    users->clear();
}

void qbeer::gotAuth(QString level)
{
//    connection->getUserById(1);
    connection->getAllUsers();
    connection->getLastTag();
    connect(timer, SIGNAL(timeout()),connection, SLOT(getLastTag()));
    timer->start(500);
    ui->labelPermission->setText(level);
}

void qbeer::insertEmptyUser()
{
    struct User user = { 0,"","","","","",0,0,0,0,1};
    connection->insertUser(user);
    users->clear();
    connection->getAllUsers();
}

qbeer::~qbeer()
{
    delete ui;
}

void qbeer::showProperties()
{
    QSettings settings;
    ConfigDialog configdialog;
    configdialog.exec();
}

void qbeer::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void qbeer::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void qbeer::closeEvent(QCloseEvent *event)
{
        writeSettings();
        event->accept();
}
        
void qbeer::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        if(this->isVisible())
            this->hide();
        else
            this->show();
    }
}
