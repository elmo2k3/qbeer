#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

    QString getAddress(void);

private slots:
    void accept();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConfigDialog *m_ui;
};

#endif // CONFIGDIALOG_H
