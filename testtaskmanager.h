#ifndef TESTTASKMANAGER_H
#define TESTTASKMANAGER_H

#include <QtTest/QTest>
#include <QObject>

#include "database.h"
#include "Task.h"
#include "taskmanager.h"

class TestTaskManager : public QObject
{
    Q_OBJECT

private slots:
    void testAddTask();

public:
    TestTaskManager();
};

#endif // TESTTASKMANAGER_H
