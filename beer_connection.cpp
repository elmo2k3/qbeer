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
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QMessageBox>
#include "beer_connection.h"

BeerConnection::BeerConnection(QObject *parent) : QTcpSocket(parent)
{
    m_last_type = -1;
    m_got_auth = 0;
    m_tag_is_new = 1;
    connect(this, SIGNAL(readyRead()), this, SLOT(evaluateData()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorBox(QAbstractSocket::SocketError)));
}
        
void BeerConnection::evaluateData(void)
{
    char buffer[1024];
    QStringList list;
    while(this->bytesAvailable())
    {
        this->readLine(buffer, 1024);
        list = QString::fromAscii(buffer).split(": ");
        if(!list[0].compare("FAIL (drink more beer)\r\n"))
        {
            QMessageBox msgBox;
            msgBox.setText("Command failed");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            break;
        }
        if(!list[0].compare("DENIED (drank too much beer?)\r\n"))
        {
            QMessageBox msgBox;
            msgBox.setText("Command denied");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            break;
        }
        if(m_last_type >= 0)
        {
            if(m_last_type == COMMAND_GET_AUTH_STRING)
            {
                if(!list[0].compare("auth_string"))
                {
                    QString foo;
                    QCryptographicHash hash(QCryptographicHash::Sha1);
                    hash.addData(m_password.toAscii());
                    foo = hash.result().toHex()+list[1].trimmed();
                    hash.reset();
                    hash.addData(foo.toAscii());
                    this->write(QString("auth "+m_user+" "+hash.result().toHex()+"\n").toAscii());
                    flush();
                    m_last_type = -1;
                }
            }
            else if(m_last_type == COMMAND_AUTH)
            {
                if(!list[0].compare("permission"))
                {
                    emit gotAuth(list[1]);
                }
                else if(!list[0].compare("OK\r\n"))
                {
                    m_got_auth = 1;
                    m_last_type = -1;
                }
            }
            else if(m_last_type == COMMAND_GET_USER_BY_ID)
            {
                if(!list[0].compare("id"))
                    m_temp_user.id = list[1].toInt();
                else if(!list[0].compare("name"))
                    m_temp_user.name = list[1].trimmed();
                else if(!list[0].compare("surname"))
                    m_temp_user.surname = list[1].trimmed();
                else if(!list[0].compare("nick"))
                    m_temp_user.nick = list[1].trimmed();
                else if(!list[0].compare("email"))
                    m_temp_user.email = list[1].trimmed();
                else if(!list[0].compare("age"))
                    m_temp_user.age = list[1].trimmed().toInt();
                else if(!list[0].compare("weight"))
                    m_temp_user.weight = list[1].trimmed().toInt();
                else if(!list[0].compare("size"))
                    m_temp_user.size = list[1].trimmed().toInt();
                else if(!list[0].compare("gender"))
                    m_temp_user.gender = list[1].trimmed().toInt();
                else if(!list[0].compare("permission"))
                {
                    m_temp_user.permission = list[1].toInt();
                    emit gotUser(m_temp_user);
                }
                else if(!list[0].compare("OK\r\n"))
                    m_last_type = -1;
            }
            else if(m_last_type == COMMAND_GET_TAG)
            {
                if(!list[0].compare("rowid"))
                    m_temp_tag.id = list[1].toInt();
                else if(!list[0].compare("tagid"))
                    m_temp_tag.tag = list[1].trimmed();
                else if(!list[0].compare("userid"))
                    m_temp_tag.userid = list[1].trimmed().toInt();
                else if(!list[0].compare("permission"))
                {
                    m_temp_tag.permission = list[1].toInt();
                    emit gotTag(m_temp_tag);
                }
                else if(!list[0].compare("OK\r\n"))
                    m_last_type = -1;
            }
            else if(m_last_type == COMMAND_LAST_TAGID)
            {
                if(!list[0].compare("time_last_tagid"))
                {
                    if(m_lastTime.compare(list[1].trimmed()))
                    {
                        m_tag_is_new = 1;
                        m_lastTime = list[1].trimmed();
                    }
                }
                else if(!list[0].compare("last_tagid"))
                {
                    if(m_lastTag.compare(list[1].trimmed()))
                    {
                        m_tag_is_new = 1;
                        m_lastTag = list[1].trimmed();
                    }
                }
                else if(!list[0].compare("user_id"))
                {
                    if(m_lastUser.compare(list[1].trimmed()))
                    {
                        m_tag_is_new = 1;
                        m_lastUser = list[1].trimmed();
                    }
                }
                else if(!list[0].compare("OK\r\n"))
                {
                    if(m_tag_is_new)
                    {
                        emit gotLastTag(m_lastTag, m_lastTime, m_lastUser);
                        m_tag_is_new = 0;
                    }
                    m_last_type = -1;
                }
            }
        }
        else if(!list[0].compare("command"))
        {
            if(!list[1].trimmed().compare("get_auth_string"))
                m_last_type = COMMAND_GET_AUTH_STRING;
            else if(!list[1].trimmed().compare("auth"))
                m_last_type = COMMAND_AUTH;
            else if(!list[1].trimmed().compare("last_tagid"))
                m_last_type = COMMAND_LAST_TAGID;
            else if(!list[1].trimmed().compare("get_all_tags"))
            {
                m_last_type = COMMAND_GET_TAG;
            }
            else if(!list[1].trimmed().compare("get_user_by_id")
                || !list[1].trimmed().compare("get_all_users"))
                m_last_type = COMMAND_GET_USER_BY_ID;
            else
                m_last_type = -1;
        }
    }
}

void BeerConnection::auth(QString user, QString password)
{
    this->write("get_auth_string\n");
    flush();
    m_user = user;
    m_password = password;
}

void BeerConnection::getUserById(qint32 id)
{
    this->write(QString("get_user_by_id "+ QString::number(id) + "\n").toAscii());
    flush();
}

void BeerConnection::getAllUsers()
{
    this->write("get_all_users\n");
    flush();
}

void BeerConnection::getAllTags()
{
    this->write("get_all_tags\n");
    flush();
}

void BeerConnection::updateUser(struct User user)
{
    QString line;
    QTextStream(&line) << "update_user \"" << user.id << "\" \"" << user.name << "\" \"" <<
        user.surname << "\" \"" << user.nick << "\" \"" << user.email << "\" \"" << user.age <<
        "\" \"" << user.weight << "\" \"" << user.size << "\" \"" << user.gender << "\" \"" << user.permission
        << "\" \"" << user.password << "\"\n";
    this->write(line.toLatin1());
    flush();
}

void BeerConnection::insertUser(struct User user)
{
    QString line;
    QTextStream(&line) << "insert_user \"" << user.name << "\" \"" <<
        user.surname << "\" \"" << user.nick << "\" \"" << user.email << "\" \"" << user.age <<
        "\" \"" << user.weight << "\" \"" << user.size << "\" \"" << user.gender << "\" \"" << user.permission
        << "\" \"" << user.password << "\"\n";
    this->write(line.toLatin1());
    flush();
}

void BeerConnection::insertUserWithTag(struct User user, QString tag, int permission)
{
    QString line;
    QTextStream(&line) << "user_insert_with_tag \"" << user.name << "\" \"" <<
        user.surname << "\" \"" << user.nick << "\" \"" << user.email << "\" \"" << user.age <<
        "\" \"" << user.weight << "\" \"" << user.size << "\" \"" << user.gender << "\" \"" << user.permission
        << "\" \"" << user.password << "\" \"" << tag << "\" \"" << permission << "\"\n";
    this->write(line.toLatin1());
    flush();
}

void BeerConnection::getLastTag()
{
    this->write("last_tagid\n");
    flush();
}

void BeerConnection::disconnect()
{
    m_got_auth = -1;
    m_last_type = -1;
    disconnectFromHost();
}


void BeerConnection::errorBox(QAbstractSocket::SocketError socketError)
{
    QMessageBox msgBox;
    msgBox.setText(this->errorString());
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}
