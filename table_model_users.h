#include <QAbstractTableModel>

class TableModelUsers : public QAbstractTableModel
{
    Q_OBJECT

    public:
        TableModelUsers(QObject *parent=0) : QAbstractTableModel(parent) {}
        
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};
