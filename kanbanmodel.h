#ifndef KANBANMODEL_H
#define KANBANMODEL_H

#include <QAbstractListModel>
#include "Column.h"
#include "IDatabaseManage.h"

class KanbanModel : public QAbstractListModel {
    Q_OBJECT

public:

    enum Roles {
        NameRole = Qt::UserRole + 1,
        TasksRole
    };

    explicit KanbanModel(IDatabaseManage *dbManager, QObject *parent = nullptr);
    ~KanbanModel();

    // Required methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addColumn(const QString &columnName);
    Q_INVOKABLE void removeColumn(int index);
    Q_INVOKABLE void renameColumn(int columnId, const QString &newColumnName);
    Q_INVOKABLE void addTask(int columnId, const QString &title, const QString &description, const QString &priority = "Low");
    Q_INVOKABLE void removeTask(int columnId, int taskId);
    Q_INVOKABLE void moveTask(int sourceColumn, int sourceTask, int targetColumn);
    Q_INVOKABLE void editTask(int columnId, int taskId, const QString &newTitle, const QString &newDescription, const QString &newPriority);

    Q_INVOKABLE bool checkUniqueness(const QString &text);
    void loadFromDatabase();

private:
    QList<Column> m_columns;
    IDatabaseManage *dbManager;
};

#endif // KANBANMODEL_H
