#ifndef QBEER_H
#define QBEER_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QStringList>
#include <QStringListModel>

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

    public slots:
        void gotAuth(QString level); 
        void gotUser(struct User user); 
    private slots:
        void showProperties();
        void saveProperties();

    private:
        Ui::qbeer *ui;
        BeerConnection *connection;
        ConfigDialog *configdialog;
};

#endif // QBEER_H
