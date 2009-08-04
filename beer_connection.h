#include <QTcpSocket>
#ifndef __BEER_CONNECTION_H__
#define __BEER_CONNECTION_H__

enum COMMAND_TYPE
{
    COMMAND_GET_AUTH_STRING,
    COMMAND_AUTH,
    COMMAND_GET_USER_BY_ID,
    COMMAND_LAST_TAGID
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
        void gotLastTag(QString lastTag, QString lastTime);

    private slots:
        void evaluateData(void);
    public slots:
        void updateUser(struct User user);
        void insertUser(struct User user);
        void disconnect(void);
        void getLastTag();

    private:
    int m_got_auth;
    int m_last_type;
    QString m_user;
    QString m_password;
    QString m_lastTag, m_lastTime;
    struct User m_temp_user;
};

#endif

