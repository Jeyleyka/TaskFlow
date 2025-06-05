#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QVector>
#include <QGridLayout>

#include "taskdialog.h"
#include "taskui.h"
#include "database.h"
#include "taskinfo.h"

class TaskManager : public QObject
{
    Q_OBJECT
public:
    TaskManager(DatabaseManager* db, QObject* parent = nullptr);

    bool createTask(Task& task);
    bool updateTask(Task& task);
    bool deleteTask(const int taskId);
    bool setTaskCompleted(int taskId, bool completed);
    QList<Task> getAllTasks() const;
    Task getTaskById(int id);

signals:
    void taskCreated(Task task);
    void taskUpdated(Task task);
    void taskDeleted(int taskId);

private:
    DatabaseManager* db;
};

#endif // TASKMANAGER_H
