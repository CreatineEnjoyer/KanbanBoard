#ifndef COLUMN_H
#define COLUMN_H

#include "taskmodel.h"

class Column
{
public:
    explicit Column(const QString &columnName, TaskModel *tasks);
    QString name;
    TaskModel *tasks;
};

#endif // COLUMN_H
