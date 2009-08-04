#ifndef QBEER_H
#define QBEER_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QStringList>
#include <QStringListModel>
#include <QSettings>
#include <QTimer>

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
        void gotLastTag(QString lastTag, QString lastTime);
        void gotConnection(void); 
        void insertEmptyUser(void);

    private:
        Ui::qbeer *ui;
        BeerConnection *connection;
        TableModelUsers *users;
        QTimer *timer;
};

#endif // QBEER_H
