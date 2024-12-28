#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include "IDatabaseManage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QVariantMap>
#include <QPair>

class DatabaseSQLite: public IDatabaseManage
{
public:
    DatabaseSQLite();
    ~DatabaseSQLite();

    void initializeDatabase() override;
    void saveColumn(const QString &name, int position) override;
    void saveTask(int columnId, const QString &title, const QString &description, const QString &priority, int position) override;

    QList<QPair<int, QString>> loadColumns() override;
    QList<QVariantMap> loadTasks(int columnId) override;

    void removeColumn(int index) override;
    void removeAllTasks(int columnId) override;
    void removeTask(int columnId, int taskId) override;

    void updateColumnPositions(int index) override;
    void updateTaskPositions(int columnId, int taskId) override;

    void renameColumn(int index, const QString &name) override;
    void updateTask(int columnId, int taskId, const QString &title, const QString &description, const QString &priority) override;
    void moveTask(int columnId, int taskId, int newColumnId, int newPosition) override;

private:
    QSqlDatabase db;
};

#endif // DATABASESQLITE_H
