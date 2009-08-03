#include <QSettings>
#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ConfigDialog)
{
    QSettings settings;
    m_ui->setupUi(this);
    m_ui->lineEditAddress->setText(settings.value("address","localhost").toString());
    m_ui->lineEditPort->setText(settings.value("port",5335).toString());
    m_ui->lineEditUser->setText(settings.value("user").toString());
    m_ui->lineEditPassword->setText(settings.value("password").toString());
}

ConfigDialog::~ConfigDialog()
{
    delete m_ui;
}

void ConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString ConfigDialog::getAddress()
{
    return m_ui->lineEditAddress->text();
}

void ConfigDialog::accept()
{
    QSettings settings;
    settings.setValue("address",m_ui->lineEditAddress->text());
    settings.setValue("port",m_ui->lineEditPort->text().toInt());
    settings.setValue("user",m_ui->lineEditUser->text());
    settings.setValue("password",m_ui->lineEditPassword->text());
    done(1);
}

