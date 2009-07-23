#include "table_model_users.h"

int TableModelUsers::rowCount(const QModelIndex &parent ) const
{
    return 5;
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
        return "asd";
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

