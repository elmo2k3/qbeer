#include <iostream>
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

    connection = new BeerConnection(this);
    TableModelUsers *users = new TableModelUsers(this);
    timer = new QTimer(this);
    
    connect(connection, SIGNAL(gotAuth(QString)), this, SLOT(gotAuth(QString)));
    connect(connection, SIGNAL(gotLastTag(QString,QString)), this, SLOT(gotLastTag(QString,QString)));
    connect(connection, SIGNAL(gotUser(struct User)), users, SLOT(insertUser(struct User)));
    connect(connection, SIGNAL(connected()), this, SLOT(gotConnection()));
    
    ui->tableViewTags->setModel(users);
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
}

void qbeer::gotAuth(QString level)
{
    ui->textEditTags->setText(level); 
//    connection->getUserById(1);
    connection->getAllUsers();
    connection->getLastTag();
    connect(timer, SIGNAL(timeout()),connection, SLOT(getLastTag()));
    timer->start(500);
}

void qbeer::gotUser(struct User user)
{
    ui->textEditTags->setText(user.name); 
}

qbeer::~qbeer()
{
    delete ui;
}

void qbeer::showProperties()
{
    QSettings settings;
    ConfigDialog configdialog;
    if(configdialog.exec())
    {
        settings.setValue("address",configdialog.getAddress());
    }
}
