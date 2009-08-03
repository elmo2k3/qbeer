#include "qbeer.h"
#include "ui_qbeer.h"

qbeer::qbeer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::qbeer)
{
    ui->setupUi(this);
    configdialog = new ConfigDialog(this);
    connection = new BeerConnection(this);
    connect(connection, SIGNAL(gotAuth(QString)), this, SLOT(gotAuth(QString)));
    connection->connectToHost("localhost", 5335);

    TableModelUsers *users = new TableModelUsers(this);
    connect(connection, SIGNAL(gotUser(struct User)), users, SLOT(insertUser(struct User)));
    ui->tableViewTags->setModel(users);
}

void qbeer::saveProperties()
{
} 

void qbeer::gotAuth(QString level)
{
    ui->textEditTags->setText(level); 
//    connection->getUserById(1);
    connection->getAllUsers();
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
    configdialog->show();
}
