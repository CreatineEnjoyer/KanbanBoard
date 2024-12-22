#include "column.h"

Column::Column(const QString &columnName, TaskModel *tasks)
    : name(columnName), tasks(tasks) {}

void Column::setColumnName(const QString &newColumnName) {
    this->name = newColumnName;
}
