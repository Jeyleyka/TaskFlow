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
#include "edittask.h"

class TaskInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TaskInfo(int id, QString titleStr, QString descStr, QString createData, TaskUI* taskWidget, QWidget* parent = nullptr);

    QString getTitle();
    QString getDesc();
    QString getPriority();
    QString getCategoryName();
    QColor getCategoryColor();
    QIcon getCategoryIcon();

    void onUpdateData();

signals:
    void onChangeUI();

private:
    TaskUI* taskUI;
    EditTask* editTaskWnd;

    QVBoxLayout* mainLayout;

    QPushButton* closeTaskInfo;
    QPushButton* deleteTask;
    QPushButton* editTask;
    QPushButton* editDesc;
    QPushButton* priority;
    QPushButton* category;

    QLabel* title;
    QLabel* description;
    QLabel* taskTime;
    QLabel* taskpriority;
    QLabel* subTask;

    int Taskid;

private slots:
    void onDeleteTaskClicked();

};

#endif // TASKINFO_H
