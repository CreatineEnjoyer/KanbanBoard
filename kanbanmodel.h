#ifndef KANBANMODEL_H
#define KANBANMODEL_H

#include <QAbstractListModel>
#include "Column.h"

class KanbanModel : public QAbstractListModel {
    Q_OBJECT

public:

    enum Roles {
        NameRole = Qt::UserRole + 1,
        TasksRole
    };

    explicit KanbanModel(QObject *parent = nullptr);
    ~KanbanModel();

    // Required methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void addColumn(const QString &columnName);
    void setColumnName(const QString &newColumnName, const QString &currentColumnName);

private:
    QList<Column> m_columns;
};

#endif // KANBANMODEL_H
