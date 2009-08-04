#include <iostream>
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
    return 10;
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
            return users[index.row()]->name;
        if(index.column() == 1)
            return users[index.row()]->surname;
        if(index.column() == 2)
            return users[index.row()]->nick;
        if(index.column() == 3)
            return users[index.row()]->email;
        if(index.column() == 4)
            return users[index.row()]->password;
        if(index.column() == 5)
        {
            switch(users[index.row()]->permission)
            {
                case 0: return "NONE";
                case 1: return "READ";
                case 2: return "ADMIN";
            }
        }
        if(index.column() == 6)
            return users[index.row()]->age;
        if(index.column() == 7)
            return users[index.row()]->weight;
        if(index.column() == 8)
            return users[index.row()]->size;
        if(index.column() == 9)
            return users[index.row()]->gender;
    }
    else
        return QVariant();
}

bool TableModelUsers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
            users[index.row()]->name = value.toString();
        if(index.column() == 1)
            users[index.row()]->surname = value.toString();
        if(index.column() == 2)
            users[index.row()]->nick = value.toString();
        if(index.column() == 3)
            users[index.row()]->email = value.toString();
        if(index.column() == 4)
            users[index.row()]->password = value.toString();
        if(index.column() == 5)
        {
            if(!value.toString().compare("NONE"))
                users[index.row()]->permission = 0;
            if(!value.toString().compare("READ"))
                users[index.row()]->permission = 1;
            if(!value.toString().compare("ADMIN"))
                users[index.row()]->permission = 2;
        }
        if(index.column() == 6)
            users[index.row()]->age = value.toInt();
        if(index.column() == 7)
            users[index.row()]->weight = value.toInt();
        if(index.column() == 8)
            users[index.row()]->size = value.toInt();
        if(index.column() == 9)
            users[index.row()]->gender = value.toInt();
    }
    emit dataChanged(index,index);
    emit updateUser(*users[index.row()]);
    return true;
}

Qt::ItemFlags TableModelUsers::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
//    if (index.row() != index.column())
            flags |= Qt::ItemIsEditable;
            return flags;
}

QVariant TableModelUsers::headerData(int section, Qt::Orientation orientation, int role) const
{
    QString header[10] = {"Name","Surname","Nick","E-Mail","Password","Permission","Age","Weight","Size","Gender"};
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
        return header[section];
    else
        return QVariant();
}

void TableModelUsers::insertUser(struct User user)
{
    struct User *user_nonvolatile = new struct User;
    memcpy(user_nonvolatile, &user, sizeof(user));
    beginInsertRows(QModelIndex(),0,0);
    users.append(user_nonvolatile);
    endInsertRows();
}

bool TableModelUsers::insertRow(int row, const QModelIndex & parent)
{
    return true;
}
