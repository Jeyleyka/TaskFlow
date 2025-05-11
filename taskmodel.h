#ifndef TASKMODEL_H
#define TASKMODEL_H

#include "task.h"

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QObject>

class TaskModel : public QAbstractTableModel
{
public:
    explicit TaskModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int rolse = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setTasks(const QList<Task> &tasks);
    Task taskAt(int row) const;
    void addTask(const Task& task);

private:
    QList<Task> m_tasks;
};

#endif // TASKMODEL_H
