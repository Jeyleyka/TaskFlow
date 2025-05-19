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
        "category_name TEXT,"
        "category_color TEXT,"
        "category_icon BLOB)";

    if (!query.exec(createTable)) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
        return false;
    }

    query.exec("CREATE TABLE IF NOT EXISTS categories ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT UNIQUE NOT NULL, "
               "color TEXT NOT NULL, "
               "icon BLOB)");

    if (!query.exec()) {
        qDebug() << "Failed to create table:" << query.lastError().text();
    }

    return true;
}


bool DatabaseManager::insertTaskToDatabase(Task &task) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO tasks (title, description, due_date, category_name, category_color, category_icon)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    // Преобразуем иконку в QByteArray
    QByteArray iconBytes;
    QPixmap pixmap = task.categoryIcon.pixmap(64, 64);
    QBuffer buffer(&iconBytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    // Привязываем значения
    query.addBindValue(task.title);
    query.addBindValue(task.description);
    QString formattedDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.addBindValue(formattedDate);
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
    QSqlQuery query("SELECT id, title, description, due_date, category_name, category_color, category_icon FROM tasks");

    while (query.next()) {
        Task task;
        task.id = query.value(0).toInt();
        task.title = query.value(1).toString();
        task.description = query.value(2).toString();

        QString datetimeStr = query.value(3).toString().trimmed();
        QDateTime dueDate = QDateTime::fromString(datetimeStr, "yyyy-MM-dd HH:mm:ss");
        if (!dueDate.isValid()) {
            qDebug() << "⚠️ Не удалось распарсить дату:" << datetimeStr;
        } else {
            task.dueDate = dueDate;
        }

        task.categoryName = query.value(4).toString();

        // Цвет
        QString colorStr = query.value(5).toString();
        task.categoryColor = QColor(colorStr);

        // Иконка
        QByteArray iconData = query.value(6).toByteArray();
        QPixmap pixmap;
        if (pixmap.loadFromData(iconData)) {
            task.categoryIcon = QIcon(pixmap);
        } else {
            qDebug() << "⚠️ Не удалось загрузить иконку из данных";
        }

        tasks.append(task);
    }

    return tasks;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
