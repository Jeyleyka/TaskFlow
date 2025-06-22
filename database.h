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
    bool updateTaskInDatabase(const Task& task);
    Task getTaskById(int id);
    QList<Task> loadTasksFromDatabase();
    QSet<QDate> getDatesWithTasksForUser(int userId);
    bool isUsersTableExists();
    static QSqlDatabase getDatabase();

private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
