#include <iostream>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include "beer_connection.h"

BeerConnection::BeerConnection(QObject *parent) : QTcpSocket(parent)
{
    m_last_type = -1;
    m_got_auth = 0;
    connect(this, SIGNAL(readyRead()), this, SLOT(evaluateData()));
}
        
void BeerConnection::evaluateData(void)
{
    char buffer[1024];
    QStringList list;
    while(this->bytesAvailable())
    {
        this->readLine(buffer, 1024);
        list = QString::fromAscii(buffer).split(": ");
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
            else if(m_last_type == COMMAND_LAST_TAGID)
            {
                if(!list[0].compare("time_last_tagid"))
                    m_lastTime = list[1].trimmed();
                else if(!list[0].compare("last_tagid"))
                    m_lastTag = list[1].trimmed();
                else if(!list[0].compare("OK\r\n"))
                {
                    emit gotLastTag(m_lastTag, m_lastTime);
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
            {
                m_last_type = COMMAND_LAST_TAGID;
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


