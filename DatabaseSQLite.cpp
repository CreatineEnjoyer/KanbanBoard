#include "DatabaseSQLite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseSQLite::DatabaseSQLite() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kanban.db");

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
    }
}

DatabaseSQLite::~DatabaseSQLite() {
    if (db.isOpen()) {
        db.close();
    }
}

void DatabaseSQLite::initializeDatabase() {
    QSqlQuery query;

    if (!query.exec("CREATE TABLE IF NOT EXISTS BoardColumn ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "position INTEGER NOT NULL);")) {
        qDebug() << "Failed to create BoardColumn table in database:" << query.lastError().text();
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Task ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "column_id INTEGER NOT NULL, "
                    "title TEXT NOT NULL, "
                    "description TEXT, "
                    "priority TEXT, "
                    "position INTEGER NOT NULL, "
                    "FOREIGN KEY(column_id) REFERENCES BoardColumn(id));")) {
        qDebug() << "Failed to create Task table in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::saveColumn(const QString &name, int position) {
    QSqlQuery query;
    query.prepare("INSERT INTO BoardColumn (name, position) "
                  "VALUES (:name, :position);");

    query.bindValue(":name", name);
    query.bindValue(":position", position);

    if (!query.exec()) {
        qDebug() << "Failed to save column in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::saveTask(int columnId, const QString &title, const QString &description, const QString &priority, int position) {
    QSqlQuery query;
    query.prepare("INSERT INTO Task (column_id, title, description, priority, position) "
                  "VALUES (:column_id, :title, :description, :priority, :position);");

    query.bindValue(":column_id", columnId);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":priority", priority);
    query.bindValue(":position", position);

    if (!query.exec()) {
        qDebug() << "Failed to save task in database:" << query.lastError().text();
    }
}

QList<QPair<int, QString>> DatabaseSQLite::loadColumns() {
    QSqlQuery query;
    QList<QPair<int, QString>> columns;

    query.exec("SELECT position, name "
               "FROM BoardColumn "
               "ORDER BY position ASC;");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        columns.append(qMakePair(id, name));
    }

    return columns;
}

QList<QVariantMap> DatabaseSQLite::loadTasks(int columnId) {
    QSqlQuery query;
    QList<QVariantMap> tasks;

    query.prepare("SELECT id, title, description, priority, position "
                  "FROM Task "
                  "WHERE column_id = :column_id "
                  "ORDER BY position ASC;");

    query.bindValue(":column_id", columnId);
    query.exec();

    while (query.next()) {
        QVariantMap task;
        task["id"] = query.value(0).toInt();
        task["title"] = query.value(1).toString();
        task["description"] = query.value(2).toString();
        task["priority"] = query.value(3).toString();
        task["position"] = query.value(4).toInt();
        tasks.append(task);
    }

    return tasks;
}

void DatabaseSQLite::removeColumn(int columnId) {
    QSqlQuery query;
    query.prepare("DELETE FROM BoardColumn "
                  "WHERE position = :position;");

    query.bindValue(":position", columnId);

    if (!query.exec()) {
        qDebug() << "Failed to delete column in database:" << query.lastError().text();
    }

    updateColumnPositions(columnId);
}

void DatabaseSQLite::removeAllTasks(int columnId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Task "
                  "WHERE column_id = :column_id;");

    query.bindValue(":column_id", columnId);

    if (!query.exec()) {
        qDebug() << "Failed to delete all tasks from column in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::removeTask(int columnId, int taskId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Task "
                  "WHERE column_id = :column_id "
                  "AND position = :position;");

    query.bindValue(":column_id", columnId);
    query.bindValue(":position", taskId);

    if (!query.exec()) {
        qDebug() << "Failed to delete task in database:" << query.lastError().text();
    }

    updateTaskPositions(columnId, taskId);
}

void DatabaseSQLite::updateColumnPositions(int index) {
    QSqlQuery query;
    query.prepare("UPDATE BoardColumn "
                  "SET position = position - 1 "
                  "WHERE position > :removedPosition;");

    query.bindValue(":removedPosition", index);

    if (!query.exec()) {
        qDebug() << "Failed to update column positions in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::updateTaskPositions(int columnId, int taskId) {
    QSqlQuery query;
    query.prepare("UPDATE Task "
                  "SET position = position - 1 "
                  "WHERE position > :removedPosition "
                  "AND column_id = :column_id;");

    query.bindValue(":column_id", columnId);
    query.bindValue(":removedPosition", taskId);

    if (!query.exec()) {
        qDebug() << "Failed to update task positions in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::renameColumn(int index, const QString &name) {
    QSqlQuery query;
    query.prepare("UPDATE BoardColumn "
                  "SET name = :column_name "
                  "WHERE position = :position;");

    query.bindValue(":column_name", name);
    query.bindValue(":position", index);

    if (!query.exec()) {
        qDebug() << "Failed to rename column in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::updateTask(int columnId, int taskId, const QString &title, const QString &description, const QString &priority) {
    QSqlQuery query;
    query.prepare("UPDATE Task "
                  "SET title = :title, description = :description, priority = :priority "
                  "WHERE position = :position "
                  "AND column_id = :column_id;");

    query.bindValue(":column_id", columnId);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":priority", priority);
    query.bindValue(":position", taskId);

    if (!query.exec()) {
        qDebug() << "Failed to update task data in database:" << query.lastError().text();
    }
}

void DatabaseSQLite::moveTask(int columnId, int taskId, int newColumnId, int newPosition) {
    QSqlQuery query;
    query.prepare("UPDATE Task "
                  "SET position = :newPosition, column_id = :newColumn "
                  "WHERE position = :position "
                  "AND column_id = :column_id;");

    query.bindValue(":column_id", columnId);
    query.bindValue(":newColumn", newColumnId);
    query.bindValue(":newPosition", newPosition);
    query.bindValue(":position", taskId);

    if (!query.exec()) {
        qDebug() << "Failed to change task position in database:" << query.lastError().text();
    }

    updateTaskPositions(columnId, taskId);
}
