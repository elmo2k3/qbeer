#include <QtGlobal>
#include "table_model_users.h"
       
TableModelUsers::TableModelUsers(QObject *parent) : QAbstractTableModel(parent)
{
//    struct User user = {1,"Biesenbach","Bjoern"};
//    users << user;
}

int TableModelUsers::rowCount(const QModelIndex &parent ) const
{
    return users.size();
}

int TableModelUsers::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant TableModelUsers::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= 5)
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return users[index.row()].name;
        if(index.column() == 1)
            return users[index.row()].surname;
        if(index.column() == 2)
            return users[index.row()].nick;
    }
    else
        return QVariant();
}

QVariant TableModelUsers::headerData(int section, Qt::Orientation orientation, int role) const
{
    QString header[10] = {"Name","Surname","nick"};
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
        return header[section];
    else
        return QVariant();
}

void TableModelUsers::insertUser(struct User user)
{
    beginInsertRows(QModelIndex(),0,1);
    users << user;
    endInsertRows();
}
