#include "database.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QCoreApplication>

QSqlDatabase DatabaseManager::db;

bool DatabaseManager::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tasks.db");

    if (!db.open()) {
        qDebug() << "Ошибка открытия БД:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString createTable =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT, "
        "description TEXT, "
        "due_date TEXT, "
        "priority TEXT, "
        "status TEXT)";

    if (!query.exec(createTable)) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
        return false;
    }

    return true;
}


bool DatabaseManager::insertTaskToDatabase(Task &task) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (title, description, due_date, priority, status) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(task.title);
    query.addBindValue(task.description);
    query.addBindValue(task.dueDate.toString("yyyy-MM-dd"));
    query.addBindValue(task.priority);
    query.addBindValue(task.status);

    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError();
        return false;
    }

    // Получаем правильный ID после вставки
    task.id = query.lastInsertId().toInt();
    qDebug() << "Задача добавлена с ID:" << task.id;  // Выводим ID новой задачи

    return true;
}


bool DatabaseManager::deleteTaskFromDatabase(int id)
{
    qDebug() << "Попытка удалить задачу с ID:" << id;  // Выводим ID задачи перед удалением

    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = ?");
    query.addBindValue(id);  // Передаём ID, а не весь объект

    if (!query.exec()) {
        qDebug() << "Ошибка удаления из базы данных:" << query.lastError();
        return false;
    }

    return true;
}


QList<Task> DatabaseManager::loadTasksFromDatabase() {
    QList<Task> tasks;
    QSqlQuery query("SELECT id, title, description, due_date, priority, status FROM tasks");

    while (query.next()) {
        Task task;
        task.id = query.value(0).toInt();
        task.title = query.value(1).toString();
        task.description = query.value(2).toString();
        task.dueDate = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
        task.priority = query.value(4).toString();
        task.status = query.value(5).toString();
        tasks.append(task);
    }

    return tasks;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
