#ifndef DATABASE_H
#define DATABASE_H

#include "Task.h"
#include "UserSession.h"

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QBuffer>

class DatabaseManager
{
public:
    bool initializeDatabase();
    bool insertTaskToDatabase(Task& task);
    bool deleteTaskFromDatabase(int id);
    bool updateTaskInDatabase(Task& task);
    QList<Task> loadTasksFromDatabase();
    static QSqlDatabase getDatabase();

private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
