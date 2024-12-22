#include "KanbanModel.h"
#include <QDebug>

KanbanModel::KanbanModel(QObject *parent)
    : QAbstractListModel(parent) {}

KanbanModel::~KanbanModel() {
    for (const Column &column : m_columns) {
        delete column.tasks;
    }
}

int KanbanModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_columns.count();
}

QVariant KanbanModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_columns.count())
        return QVariant();

    const Column &column = m_columns[index.row()];

    switch (role) {
        case NameRole:
            return column.name;
        case TasksRole:
            return QVariant::fromValue(column.tasks);
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> KanbanModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TasksRole] = "tasks";
    return roles;
}

bool KanbanModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool ret = false;

    if(index.row() >= 0 && index.row() < m_columns.count()) {
        Column &column = m_columns[index.row()];

        switch (role) {
            case NameRole:
                column.setColumnName(value.toString());
                ret = true;
                break;
            default:
                return false;
        }

        if (ret)
            emit dataChanged(index, index);
    }

    return ret;
}

void KanbanModel::addColumn(const QString &columnName) {
    Column column(columnName, new TaskModel(this));
    beginInsertRows(QModelIndex(), m_columns.size(), m_columns.size());
    m_columns.append(column);
    endInsertRows();
}
