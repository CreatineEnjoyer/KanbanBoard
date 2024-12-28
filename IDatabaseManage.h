#ifndef IDATABASEMANAGE_H
#define IDATABASEMANAGE_H

#include <QString>

class IDatabaseManage {
public:
    virtual ~IDatabaseManage() {};

    virtual void initializeDatabase() = 0;
    virtual void saveColumn(const QString &name, int position) = 0;
    virtual void saveTask(int columnId, const QString &title, const QString &description, const QString &priority, int position) = 0;

    virtual QList<QPair<int, QString>> loadColumns() = 0;
    virtual QList<QVariantMap> loadTasks(int columnId) = 0;

    virtual void removeColumn(int index) = 0;
    virtual void removeAllTasks(int columnId) = 0;
    virtual void removeTask(int columnId, int taskId) = 0;

    virtual void updateColumnPositions(int index) = 0;
    virtual void updateTaskPositions(int columnId, int taskId) = 0;

    virtual void renameColumn(int index, const QString &name) = 0;
    virtual void updateTask(int columnId, int taskId, const QString &title, const QString &description, const QString &priority) = 0;
    virtual void moveTask(int columnId, int taskId, int newColumnId, int newPosition) = 0;

};
#endif // IDATABASEMANAGE_H
