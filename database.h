#ifndef DATABASE_H
#define DATABASE_H

#include "Task.h"

#include <QString>
#include <QtSql/QSqlDatabase>

class DatabaseManager
{
public:
    bool initializeDatabase();
    bool insertTaskToDatabase(const Task& task);
    QList<Task> loadTasksFromDatabase();
    static QSqlDatabase getDatabase();

private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
