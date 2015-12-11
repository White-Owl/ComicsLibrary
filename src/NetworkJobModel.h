#ifndef NETWORKJOBMODEL_H
#define NETWORKJOBMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class NetworkJobModel : public QAbstractItemModel {
    QStringList headerTitles;

public:
    NetworkJobModel();
    ~NetworkJobModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
};

#endif // NETWORKJOBMODEL_H
