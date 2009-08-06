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

#include <QTcpSocket>
#ifndef __BEER_CONNECTION_H__
#define __BEER_CONNECTION_H__

enum COMMAND_TYPE
{
    COMMAND_GET_AUTH_STRING,
    COMMAND_AUTH,
    COMMAND_GET_USER_BY_ID,
    COMMAND_LAST_TAGID,
    COMMAND_GET_TAG
};

struct User
{
    int id;
    QString name;
    QString surname;
    QString nick;
    QString email;
    QString password;
    qint32 age;
    qint32 weight;
    qint32 size;
    qint32 gender;
    qint32 permission;
};

struct Tag
{
    int id;
    QString tag;
    int userid;
    int permission;
};

class BeerConnection : public QTcpSocket
{
    Q_OBJECT

    public:
        BeerConnection(QObject *parent =0);

        void auth(QString user, QString password);
        void getUserById(qint32 id);
        void getAllUsers();
        void getAllTags();

    signals:
        void gotAuth(QString string);
        void gotUser(struct User user);
        void gotTag(struct Tag tag);
        void gotLastTag(QString lastTag, QString lastTime, QString lastUserId);

    private slots:
        void evaluateData(void);
        void errorBox(QAbstractSocket::SocketError socketError);
    public slots:
        void updateUser(struct User user);
        void insertUser(struct User user);
        void insertUserWithTag(struct User user, QString tag, int permission);
        void disconnect(void);
        void getLastTag();

    private:
    int m_got_auth;
    int m_last_type;
    QString m_user;
    QString m_password;
    QString m_lastTag, m_lastTime, m_lastUser;
    bool m_tag_is_new;
    struct User m_temp_user;
    struct Tag m_temp_tag;
};

#endif

