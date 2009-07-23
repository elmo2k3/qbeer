#include "qbeer.h"
#include "ui_qbeer.h"
#include "table_model_users.h"

qbeer::qbeer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::qbeer)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    tcpSocket->connectToHost("localhost", 5335);
    tcpSocket->write("get_auth_string\n");
    TableModelUsers *users = new TableModelUsers(this);
    ui->tableViewTags->setModel(users);
}

void qbeer::dataReceived()
{
    char buffer[1024];
    QStringList list;
    QString foo;
    while(tcpSocket->bytesAvailable())
    {
        tcpSocket->readLine(buffer, 1024);
        list = QString::fromAscii(buffer).split(":");
        if(!list[0].compare("auth_string"))
        {
            QCryptographicHash hash(QCryptographicHash::Sha1);
            ui->textEditTags->setText(list[1].trimmed());
            foo = "8b7d380524c8497e1614216ee2acdf2cb9cc4fb2"+list[1].trimmed();
            hash.addData(foo.toAscii());
            QString bla(hash.result().toHex());
            ui->textEditTags->setText(list[1].trimmed() +"\t"+ bla); 
            tcpSocket->write(QString("auth elmo "+bla+"\n").toAscii());
        }
        else if(!list[0].compare("permission") && !list[1].trimmed().compare("ADMIN"))
        {
            ui->textEditTags->setText(list[1]); 
        }

            

//        string += string.fromAscii(buffer);
//        ui->textEditTags->setText(string); 
    }
}

qbeer::~qbeer()
{
    delete ui;
}
