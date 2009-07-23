#ifndef QBEER_H
#define QBEER_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QStringList>
#include <QStringListModel>

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
    void dataReceived(); 

private:
    Ui::qbeer *ui;
    QTcpSocket *tcpSocket;
};

#endif // QBEER_H
