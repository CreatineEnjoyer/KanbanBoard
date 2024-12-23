#include "column.h"

Column::Column(const QString &columnName, TaskModel *tasks)
    : name(columnName), tasks(tasks) {}
