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
        "user_id INTEGER, "
        "title TEXT, "
        "description TEXT, "
        "due_date TEXT, "
        "priority INTEGER, "
        "completed INTEGER DEFAULT 0,"
        "category_name TEXT,"
        "category_color TEXT,"
        "category_icon BLOB,"
        "FOREIGN KEY (user_id) REFERENCES user(id) )";

    if (!query.exec(createTable)) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
        return false;
    }

    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT UNIQUE NOT NULL, "
               "password TEXT NOT NULL, "
               "icon BLOB)");

    query.prepare("CREATE TABLE IF NOT EXISTS categories ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "user_id INTEGER, "
                  "name TEXT UNIQUE NOT NULL, "
                  "color TEXT NOT NULL, "
                  "icon BLOB, "
                  "FOREIGN KEY (user_id) REFERENCES user(id) )");

    if (!query.exec())
        qDebug() << "Failed to create table:" << query.lastError().text();

    return true;
}


bool DatabaseManager::insertTaskToDatabase(Task &task) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO tasks (user_id, title, description, due_date, priority, completed, category_name, category_color, category_icon)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    // Преобразуем иконку в QByteArray
    QByteArray iconBytes;
    QPixmap pixmap = task.categoryIcon.pixmap(64, 64);
    QBuffer buffer(&iconBytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    // Привязываем значения
    query.addBindValue(UserSession::getUserId());
    query.addBindValue(task.title);
    query.addBindValue(task.description);
    QString formattedDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.addBindValue(formattedDate);
    query.addBindValue(task.priority);
    query.addBindValue(task.completed);
    query.addBindValue(task.categoryName);
    query.addBindValue(task.categoryColor.name());  // Цвет в формате "#RRGGBB"
    query.addBindValue(iconBytes);

    qDebug() << "SQL:" << query.lastQuery();
    qDebug() << "Bound Values:" << query.boundValues();

    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError();
        return false;
    }

    task.id = query.lastInsertId().toInt();
    qDebug() << "Задача добавлена с ID:" << task.id;

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
    QSqlQuery query;

    query.prepare("SELECT * FROM tasks WHERE user_id = :user_id");
    query.bindValue(":user_id", UserSession::getUserId());

    if (query.exec())
    {
        while (query.next()) {
            Task task;
            task.id = query.value(0).toInt();
            // task.user_id = query.value(1).toInt();
            task.title = query.value(2).toString();
            task.description = query.value(3).toString();

            QString datetimeStr = query.value(4).toString().trimmed();
            QDateTime dueDate = QDateTime::fromString(datetimeStr, "yyyy-MM-dd HH:mm:ss");
            if (!dueDate.isValid()) {
                qDebug() << "⚠️ Не удалось распарсить дату:" << datetimeStr;
            } else {
                task.dueDate = dueDate;
            }

            task.priority = query.value(5).toInt();
            task.completed = query.value(6).toInt();
            task.categoryName = query.value(7).toString();

            // Цвет
            QString colorStr = query.value(8).toString();
            task.categoryColor = QColor(colorStr);

            // Иконка
            QByteArray iconData = query.value(9).toByteArray();
            QPixmap pixmap;
            if (pixmap.loadFromData(iconData)) {
                task.categoryIcon = QIcon(pixmap);
            } else {
                qDebug() << "⚠️ Не удалось загрузить иконку из данных";
            }

            tasks.append(task);
        }
    }

    return tasks;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
