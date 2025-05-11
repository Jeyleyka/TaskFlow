#include "database.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QCoreApplication>

QSqlDatabase DatabaseManager::db;

bool DatabaseManager::initializeDatabase() {
    QString dbPath = QCoreApplication::applicationDirPath() + "/tasks.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Data base connection error: " << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            due_date TEXT,
            priority TEXT,
            status TEXT
        );
    )";

    if (!query.exec(createTable)) {
        qCritical() << "create table error: " << query.lastError().text();
        return false;
    }

    qDebug() << "Data base and table initialized";
    return true;
}

bool DatabaseManager::insertTaskToDatabase(const Task &task) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (title, description, due_date, priority, status) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(task.title);
    query.addBindValue(task.description);
    query.addBindValue(task.dueDate.toString("yyyy-MM-dd"));
    query.addBindValue(task.priority);
    query.addBindValue(task.status);

    qDebug() << "Загрузка задач из БД:";
    while (query.next()) {
        qDebug() << query.value(0).toString(); // title
    }

    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError();
        return false;
    }

    return true;
}

QList<Task> DatabaseManager::loadTasksFromDatabase() {
    QList<Task> tasks;
    QSqlQuery query("SELECT title, description, due_date, priority, status FROM tasks");

    while (query.next()) {
        Task task;
        task.title = query.value(0).toString();
        task.description = query.value(1).toString();
        task.dueDate = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        task.priority = query.value(3).toString();
        task.status = query.value(4).toString();
        tasks.append(task);
    }

    return tasks;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
