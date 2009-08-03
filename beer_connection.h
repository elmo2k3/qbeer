#include <QTcpSocket>
#ifndef __BEER_CONNECTION_H__
#define __BEER_CONNECTION_H__

enum COMMAND_TYPE
{
    COMMAND_GET_AUTH_STRING,
    COMMAND_AUTH,
    COMMAND_GET_USER_BY_ID
};

struct User
{
    qint32 id;
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

class BeerConnection : public QTcpSocket
{
    Q_OBJECT

    public:
        BeerConnection(QObject *parent =0);

        void auth(QString user, QString password);
        void getUserById(qint32 id);
        void getAllUsers();

    signals:
        void gotAuth(QString string);
        void gotUser(struct User user);

    private slots:
        void evaluateData(void);

    private:
    int m_got_auth;
    int m_last_type;
    QString m_user;
    QString m_password;
    struct User m_temp_user;
};

#endif

