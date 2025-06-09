#include "taskmanager.h"

TaskManager::TaskManager(DatabaseManager* db, QObject* parent)
    : QObject(parent), db(db)
{
    if (!this->db) {
        qWarning() << "TaskManager: database pointer is null!";
    }
}

bool TaskManager::createTask(Task &task) {
    if (!this->db) {
        qWarning() << "TaskManager: cannot create task, db is null";
        return false;
    }

    if (this->db->insertTaskToDatabase(task)) {
        emit this->taskCreated(task);
        return true;
    }

    return false;
}

QList<Task> TaskManager::getAllTasks() const {
    if (!this->db) {
        qWarning() << "TaskManager: cannot fetch tasks, db is null";
        return {};
    }

    return db->loadTasksFromDatabase();
}

Task TaskManager::getTaskById(int taskId) {
    QSqlQuery query;
    query.prepare("SELECT id, user_id, title, description, due_date, priority, completed, category_name, category_color, category_icon FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    if (query.exec() && query.next()) {
        Task task;
        task.id = query.value("id").toInt();
        task.user_id = query.value("user_id").toInt();
        task.title = query.value("title").toString();
        task.description = query.value("description").toString();
        task.dueDate = query.value("due_date").toDateTime();
        task.priority = query.value("priority").toInt();
        task.completed = query.value("completed").toBool();
        task.categoryName = query.value("category_name").toString();
        task.categoryColor = QColor(query.value("category_color").toString());

        QByteArray iconData = query.value("category_icon").toByteArray();
        QPixmap pixmap;
        pixmap.loadFromData(iconData);
        task.categoryIcon = QIcon(pixmap);

        return task;
    } else {
        qDebug() << "getTaskById: Task not found or error:" << query.lastError().text();
        return Task();  // Возвращаем пустой объект, чтобы избежать краша
    }
}

bool TaskManager::updateTask(Task& task) {
    if (!this->db) {
        qWarning() << "TaskManager: cannot update task, db is null";
        return false;
    }

    if (this->db->updateTaskInDatabase(task)) {
        emit this->taskUpdated(task);
        return true;
    }

    return false;
}

bool TaskManager::deleteTask(const int taskId) {
    if (!this->db) {
        qWarning() << "TaskManager: cannot delete task, db is null";
        return false;
    }

    if (this->db->deleteTaskFromDatabase(taskId)) {
        emit this->taskDeleted(taskId);
        return true;
    }

    return false;
}

bool TaskManager::setTaskCompleted(int taskId, bool completed) {
    static bool updating = false;
    if (updating) return true; // предотвращает рекурсию

    updating = true;

    QSqlQuery query;
    query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
    query.bindValue(":completed", completed);
    query.bindValue(":id", taskId);

    if (query.exec()) {
        Task updatedTask = getTaskById(taskId);
        emit taskUpdated(updatedTask);
        updating = false;
        return true;
    } else {
        qDebug() << "Ошибка при обновлении completed:" << query.lastError().text();
        updating = false;
        return false;
    }
}
