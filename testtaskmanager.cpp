#include "testtaskmanager.h"

TestTaskManager::TestTaskManager() {}

void TestTaskManager::testAddTask() {
    DatabaseManager db;
    db.initializeDatabase();

    TaskManager manager(&db);

    Task task;
    task.title = "Test Task";
    task.description = "Description";
    task.priority = 1;
    task.completed = true;

    bool result = manager.createTask(task);

    QVERIFY(result);

    const QList<Task>& tasks = manager.getAllTasks();

    QCOMPARE(tasks.size(), 2);
    QCOMPARE(tasks.last().title, QString("Test Task"));
    QCOMPARE(tasks.last().description, QString("Description"));
}

QTEST_MAIN(TestTaskManager)
#include "testtaskmanager.moc"
