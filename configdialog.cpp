/*
 * Copyright (C) 2009 Bjoern Biesenbach <bjoern@bjoern-b.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

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
    m_ui->checkBoxUnknownTag->setChecked(settings.value("popup_on_unknown_tag",true).toBool());
    m_ui->checkBoxOwnTag->setChecked(settings.value("popup_on_own_tag",true).toBool());
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
    settings.setValue("popup_on_unknown_tag",m_ui->checkBoxUnknownTag->isChecked());
    settings.setValue("popup_on_own_tag",m_ui->checkBoxOwnTag->isChecked());
    done(1);
}

