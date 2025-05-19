#include "taskmodel.h"

TaskModel::TaskModel(QObject* parent): QAbstractTableModel(parent) {

}

int TaskModel::rowCount(const QModelIndex& parent) const {
    return this->m_tasks.size();
}

int TaskModel::columnCount(const QModelIndex& parent) const {
    return 4; // title, description, dueDate,
}

QVariant TaskModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= this->m_tasks.size())
        return QVariant();

    const Task& task = this->m_tasks.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return task.title;
            case 1: return task.description;
            case 2: return task.dueDate.toString("yyyy-MM-dd");
            case 5: return "";
            default: return QVariant();
        }
    }

    return QVariant();
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Title";
            case 1: return "Description";
            case 2: return "Due Date";
            default: return QVariant();
        }
    }

    return QVariant();
}

void TaskModel::setTasks(const QList<Task> &tasks) {
    beginResetModel();
    this->m_tasks = tasks;
    endResetModel();
}

Task TaskModel::taskAt(int row) const {
    if (row >= 0 && row < this->m_tasks.size()) {
        qDebug() << "VALID task\n";
        return this->m_tasks.at(row);
    }

    qDebug() << "INVALID task\n";
    return Task{};
}

void TaskModel::addTask(const Task &task) {
    this->beginInsertRows(QModelIndex(), this->m_tasks.size(), this->m_tasks.size());
    this->m_tasks.append(task);
    this->endInsertRows();
}

void TaskModel::removeTask(int row) {
    if (row < 0 || row >= this->m_tasks.size() + 1) {
        qDebug() << "Failed to delete task from database";
    }

    beginRemoveRows(QModelIndex(), row, row);
    this->m_tasks.removeAt(row);
    endRemoveRows();
}

int TaskModel::getTaskID(Task& task) {
    return task.id;
}
