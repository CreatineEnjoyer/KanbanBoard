#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>

class TaskModel: public QAbstractListModel {
    Q_OBJECT
public:
    struct Task {
        QString title;
        QString description;
        int priority;
    };

    enum TaskRoles {
        TitleRole = Qt::UserRole + 1,
        DescriptionRole,
        PriorityRole
    };

    explicit TaskModel(QObject *parent = nullptr);

    // Required methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addTask(const QString &title, const QString &description, int priority = 3);
    Q_INVOKABLE void removeTask(int index);

private:
    QList<Task> m_tasks;
};

#endif // TASKMODEL_H
