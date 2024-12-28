#include "KanbanModel.h"
#include <QDebug>

KanbanModel::KanbanModel(IDatabaseManage *dbManager, QObject *parent)
    : QAbstractListModel(parent), dbManager(dbManager) {
    loadFromDatabase();
}

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

    dbManager->saveColumn(columnName, m_columns.size() - 1);
}

void KanbanModel::removeColumn(int index) {
    if (index < 0 || index >= m_columns.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    delete m_columns[index].tasks;
    m_columns.removeAt(index);
    endRemoveRows();

    dbManager->removeAllTasks(index);
    dbManager->removeColumn(index);
}

void KanbanModel::renameColumn(int columnId, const QString &newColumnName) {
    if (columnId < 0 || columnId >= m_columns.size())
        return;

    m_columns[columnId].name = newColumnName;

    dbManager->renameColumn(columnId, newColumnName);

    QModelIndex index = createIndex(columnId, 0);
    emit dataChanged(index, index, {NameRole});
}


void KanbanModel::addTask(int columnId, const QString &title, const QString &description, const QString &priority) {
    TaskModel *targetTasks = m_columns[columnId].tasks;
    targetTasks->addTask(title, description, priority);

    dbManager->saveTask(columnId, title, description, priority, targetTasks->getTasks().size() - 1);
}

void KanbanModel::removeTask(int columnId, int taskId) {
    TaskModel *targetTasks = m_columns[columnId].tasks;
    targetTasks->removeTask(taskId);

    dbManager->removeTask(columnId, taskId);
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

    dbManager->moveTask(sourceColumn, sourceTask, targetColumn, targetTasks->getTasks().size());

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

    dbManager->updateTask(columnId, taskId, newTitle, newDescription, newPriority);
}

void KanbanModel::loadFromDatabase() {
    dbManager->initializeDatabase();

    // Load all columns
    QList<QPair<int, QString>> columns = dbManager->loadColumns();

    // For each column load tasks
    for (const QPair<int, QString> &columnData : columns) {
        int columnId = columnData.first;
        QString columnName = columnData.second;

        Column column(columnName, new TaskModel(this));

        // Load tasks for this column
        QList<QVariantMap> tasks = dbManager->loadTasks(columnId);

        for (const QVariantMap &taskData : tasks) {
            QString title = taskData["title"].toString();
            QString description = taskData["description"].toString();
            QString priority = taskData["priority"].toString();

            // Add task to TaskModel in this column
            column.tasks->addTask(title, description, priority);
        }

        // Add task to this model
        beginInsertRows(QModelIndex(), m_columns.size(), m_columns.size());
        m_columns.append(column);
        endInsertRows();
    }
}
