#include "main.h"
#include "NetworkJobModel.h"

NetworkJobModel::NetworkJobModel() {
    headerTitles << "Title" << "Chapter" << "Page" << "Status";
}

NetworkJobModel::~NetworkJobModel() { }

QModelIndex NetworkJobModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column);
}
QModelIndex NetworkJobModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

int NetworkJobModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return headerTitles.size();
}

QVariant NetworkJobModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation==Qt::Horizontal && role==Qt::DisplayRole) {
        return headerTitles[section];
    }
    return QVariant();
}

QVariant NetworkJobModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) return QVariant();
    Q_UNUSED(role);
    return QVariant();
}

int NetworkJobModel::rowCount(const QModelIndex &parent) const {
    if(!parent.isValid()) {
        return comicsSources.size();
    } else {
        return
    }
    return 0;
}

