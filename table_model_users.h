#include <QList>
#include <QAbstractTableModel>
#include "beer_connection.h"

#ifndef __TABLE_MODEL_USERS_H__
#define __TABLE_MODEL_USERS_H__

class TableModelUsers : public QAbstractTableModel
{
    Q_OBJECT

    public:
        TableModelUsers(QObject *parent=0); 
        
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    public slots:
        void insertUser(struct User user);

    private:
        QList<struct User> users;
};

#endif
