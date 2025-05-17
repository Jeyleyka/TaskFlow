#ifndef TASKINFO_H
#define TASKINFO_H

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>

#include "taskui.h"

class TaskInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TaskInfo(int id, QString priorityIco, QString titleStr, QString descStr, QString createData, QString priorityS, TaskUI* taskWidget, QWidget* parent = nullptr);

private:
    TaskUI* taskUI;

    QVBoxLayout* mainLayout;

    QPushButton* closeTaskInfo;
    QPushButton* deleteTask;
    QPushButton* editTask;
    QPushButton* editDesc;

    QLabel* title;
    QLabel* description;
    QLabel* taskTime;
    QLabel* taskpriority;
    QLabel* subTask;

    int Taskid;

    void onDeleteTaskClicked();

};

#endif // TASKINFO_H
