#ifndef TASKINFO_H
#define TASKINFO_H

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class TaskInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TaskInfo(QString priorityIco, QString titleStr, QString descStr, QString createData, QString priorityS, QWidget* parent = nullptr);

private:
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

};

#endif // TASKINFO_H
