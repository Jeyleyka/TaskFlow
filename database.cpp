#include "database.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QCoreApplication>

QSqlDatabase DatabaseManager::db;

bool DatabaseManager::initializeDatabase() {
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("tasks.db");
        if (!db.open()) {
            qDebug() << "Не удалось открыть базу данных:" << db.lastError().text();
        } else {
            qDebug() << "База данных успешно открыта";
        }
    } else {
        // Соединение уже существует, можно просто убедиться, что оно открыто
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (!db.isOpen()) {
            if (!db.open()) {
                qDebug() << "Не удалось повторно открыть базу данных:" << db.lastError().text();
            }
        }
    }

    QSqlQuery query;

    QString createTable =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER, "
        "title TEXT, "
        "description TEXT, "
        "creation_date TEXT, "
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
                  "name TEXT NOT NULL, "
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
        INSERT INTO tasks (user_id, title, description, creation_date, due_date, priority, completed, category_name, category_color, category_icon)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
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

    query.addBindValue(task.creationDate.toString("yyyy-MM-dd HH:mm:ss"));
    QString formattedDate = task.dueDate.toString("yyyy-MM-dd HH:mm:ss");
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
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);  // Передаём ID, а не весь объект

    if (!query.exec()) {
        qDebug() << "Ошибка удаления из базы данных:" << query.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::updateTaskInDatabase(Task& task) {
    QSqlQuery query;

    query.prepare(R"(
        UPDATE tasks
        SET title = :title,
            description = :description,
            due_date = :due_date,
            is_completed = :is_completed,
            category_name = :category_name,
            category_color = :category_color,
            category_icon = :category_icon
        WHERE id = :id
    )");

    query.bindValue(":title", task.title);
    query.bindValue(":description", task.description);
    query.bindValue(":due_date", task.dueDate.toString(Qt::ISODate));
    query.bindValue(":is_completed", task.completed);
    query.bindValue(":category_name", task.categoryName);
    query.bindValue(":category_color", task.categoryColor.name()); // сохраняем как строку "#RRGGBB"

    QByteArray iconData;
    QBuffer buffer(&iconData);
    buffer.open(QIODevice::WriteOnly);
    task.categoryIcon.pixmap(32, 32).save(&buffer, "PNG");
    query.bindValue(":category_icon", iconData);

    query.bindValue(":id", task.id);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления задачи:" << query.lastError().text();
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

            QString creationDatetimeStr = query.value(4).toString().trimmed();
            QDateTime creationDate = QDateTime::fromString(creationDatetimeStr, "yyyy-MM-dd HH:mm:ss");
            if (!creationDate.isValid()) {
                qDebug() << "⚠️ Не удалось распарсить дату:" << creationDatetimeStr;
            } else {
                task.creationDate = creationDate;
            }

            QString datetimeStr = query.value(5).toString().trimmed();
            QDateTime dueDate = QDateTime::fromString(datetimeStr, "yyyy-MM-dd HH:mm:ss");
            if (!dueDate.isValid()) {
                qDebug() << "⚠️ Не удалось распарсить дату:" << datetimeStr;
            } else {
                task.dueDate = dueDate;
            }

            task.priority = query.value(6).toInt();
            task.completed = query.value(7).toInt();
            task.categoryName = query.value(8).toString();

            // Цвет
            QString colorStr = query.value(9).toString();
            task.categoryColor = QColor(colorStr);

            // Иконка
            QByteArray iconData = query.value(10).toByteArray();
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

QSet<QDate> DatabaseManager::getDatesWithTasksForUser(int userId)
{
    QSet<QDate> dates;
    QSqlQuery query;
    query.prepare(R"(
        SELECT DISTINCT DATE(due_date) FROM tasks
        WHERE user_id = ?
    )");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            QString dateStr = query.value(0).toString(); // "yyyy-MM-dd"
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (date.isValid()) {
                dates.insert(date);
            }
        }
    } else {
        qDebug() << "getDatesWithTasksForUser error:" << query.lastError();
    }
    return dates;
}


QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
