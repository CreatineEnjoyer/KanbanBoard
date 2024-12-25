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

void KanbanModel::addColumn(const QString &columnName) {
    Column column(columnName, new TaskModel(this));
    beginInsertRows(QModelIndex(), m_columns.size(), m_columns.size());
    m_columns.append(column);
    endInsertRows();
}

void KanbanModel::removeColumn(int index) {
    if (index < 0 || index >= m_columns.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    delete m_columns[index].tasks;
    m_columns.removeAt(index);
    endRemoveRows();
}

void KanbanModel::renameColumn(int columnId, const QString &newColumnName) {
    if (columnId < 0 || columnId >= m_columns.size())
        return;

    m_columns[columnId].name = newColumnName;

    QModelIndex index = createIndex(columnId, 0);
    emit dataChanged(index, index, {NameRole});
}


void KanbanModel::addTask(int columnId, const QString &title, const QString &description, const QString &priority) {
    TaskModel *targetTasks = m_columns[columnId].tasks;
    targetTasks->addTask(title, description, priority);
}

void KanbanModel::removeTask(int columnId, int taskId) {
    TaskModel *targetTasks = m_columns[columnId].tasks;
    targetTasks->removeTask(taskId);
}

void KanbanModel::moveTask(int sourceColumn, int sourceTask, int targetColumn) {
    if (sourceColumn < 0 || sourceColumn >= m_columns.size() ||
        targetColumn < 0 || targetColumn >= m_columns.size()) {
        return;
    }

    TaskModel *sourceTasks = m_columns[sourceColumn].tasks;
    TaskModel *targetTasks = m_columns[targetColumn].tasks;

    if (sourceTask < 0 || sourceTask >= sourceTasks->rowCount()) {
        return;
    }

    // Retrieve task details
    QString title = sourceTasks->data(sourceTasks->index(sourceTask), TaskModel::TitleRole).toString();
    QString description = sourceTasks->data(sourceTasks->index(sourceTask), TaskModel::DescriptionRole).toString();
    QString priority = sourceTasks->data(sourceTasks->index(sourceTask), TaskModel::PriorityRole).toString();

    // Remove task from source
    sourceTasks->removeTask(sourceTask);

    // Add task to target
    targetTasks->addTask(title, description, priority);
}

void KanbanModel::editTask(int columnId, int taskId, const QString &newTitle, const QString &newDescription, const QString &newPriority) {
    TaskModel *targetTasks = m_columns[columnId].tasks;
    targetTasks->editTask(taskId, newTitle, newDescription, newPriority);
}
