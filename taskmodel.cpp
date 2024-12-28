#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent) : QAbstractListModel(parent) {}


int TaskModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_tasks.size();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_tasks.size())
        return QVariant();

    const Task &task = m_tasks[index.row()];

    switch (role) {
        case TitleRole:
            return task.title;
        case DescriptionRole:
            return task.description;
        case PriorityRole:
            return task.priority;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> TaskModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[PriorityRole] = "priority";
    return roles;
}

void TaskModel::addTask(const QString &title, const QString &description, const QString &priority) {
    beginInsertRows(QModelIndex(), m_tasks.size(), m_tasks.size());
    m_tasks.append({title, description, priority});
    endInsertRows();
}

void TaskModel::removeTask(int index) {
    if (index < 0 || index >= m_tasks.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_tasks.removeAt(index);
    endRemoveRows();
}

void TaskModel::editTask(int index, const QString &newTitle, const QString &newDescription, const QString &newPriority) {
    if (index < 0 || index >= m_tasks.size())
        return;

    m_tasks[index].title = newTitle;
    m_tasks[index].description = newDescription;
    m_tasks[index].priority = newPriority;

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {TitleRole, DescriptionRole, PriorityRole});
}

QList<TaskModel::Task> TaskModel::getTasks() {
    return this->m_tasks;
}

