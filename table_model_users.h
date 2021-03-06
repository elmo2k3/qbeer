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
        bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool insertRow(int row, const QModelIndex & parent = QModelIndex());
        struct User *getUserById(int id);

    signals:
        void updateUser(struct User user);

    public slots:
        void insertUser(struct User user);
        void insertTag(struct Tag tag);
        void clear();

    private:
        QList<struct User> users;
        QList<struct Tag> tags;
};

#endif
