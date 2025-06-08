#include "indexwnd.h"

IndexWnd::IndexWnd(TaskManager* taskManager, QWidget* parent)
    : QWidget(parent), taskManager(taskManager)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(0);

    ThemeManager::instance().loadTheme();

    // --- Заголовок и иконка профиля ---
    this->titleLayout = new QHBoxLayout();

    // Левая заглушка (для симметрии)
    QWidget* leftSpacer = new QWidget(this);
    leftSpacer->setFixedWidth(40);  // как отступ

    // Центр – заголовок
    this->titleLayout = new QHBoxLayout();
    this->titleLayout->setContentsMargins(0, 20, 0, 20);
    this->titleLayout->setSpacing(0);

    // Создаём заголовок
    QLabel* indexTitle = new QLabel(tr("Index"), this);
    indexTitle->setStyleSheet("font-size: 19px; margin-left: 310px; padding-right: 250px; color: #fff;");
    indexTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    indexTitle->setAlignment(Qt::AlignCenter); // Центрируем текст внутри QLabel

    // Создаём кнопку-иконку
    QSqlQuery query;
    query.prepare("SELECT icon FROM users WHERE id = :id");
    query.bindValue(":id", UserSession::getUserId());

    if (query.exec() && query.next()) {
        QPixmap pixmap;
        pixmap.loadFromData(query.value("icon").toByteArray());

        this->iconBtn = new QPushButton(this);
        this->iconBtn->setFixedSize(54, 54);
        this->iconBtn->setIcon(pixmap.scaled(54, 54, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this->iconBtn->setIconSize(QSize(54, 54));
        this->iconBtn->setStyleSheet("border: none;");
    } else {
        qDebug() << "Ошибка запроса иконки пользователя:" << query.lastError().text();
    }

    // Создаём контейнер с layout по центру
    QHBoxLayout* wrapperLayout = new QHBoxLayout();
    wrapperLayout->setContentsMargins(0, 0, 0, 20);
    wrapperLayout->addStretch();         // Раздвигаем влево
    wrapperLayout->addWidget(indexTitle);
    wrapperLayout->addSpacing(10);       // Промежуток между текстом и кнопкой
    if (this->iconBtn) {
        wrapperLayout->addWidget(this->iconBtn);
    }
    wrapperLayout->addStretch();         // Раздвигаем вправо

    // Оборачиваем всё в виджет, чтобы потом легко вставить в основной layout
    QWidget* wrapperWidget = new QWidget(this);
    wrapperWidget->setLayout(wrapperLayout);

    // Добавляем в основной layout
    layout->addWidget(wrapperWidget);


    // --- Поиск ---
    this->initSearch();
    QVBoxLayout* searchLayout = new QVBoxLayout;
    searchLayout->addWidget(this->search, 0, Qt::AlignHCenter);
    searchLayout->setContentsMargins(0, 0, 0, 20);
    layout->addLayout(searchLayout);

    // --- Активные задачи ---
    this->todayTasksBtn = new QPushButton(tr("Today:"), this);
    this->todayTasksBtn->setStyleSheet("width: 100px; height: 31px; border-radius: 5px; background-color: #292929;");
    this->tasksLayout = new QVBoxLayout;
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0, 30, 0, 0);
    this->tasksLayout->addWidget(this->todayTasksBtn, 0, Qt::AlignHCenter);
    layout->addLayout(this->tasksLayout);

    // --- Профиль ---
    this->profile = new ProfileWnd;

    // --- Завершённые задачи ---
    this->initSortTags();  // Создаёт completedTasksBtn и заполняет completedTasks
    this->completeTaskslayout = new QVBoxLayout;
    this->completeTaskslayout->setSpacing(15);
    this->completeTaskslayout->setContentsMargins(0, 20, 0, 0);
    this->completeTaskslayout->addWidget(this->completedTasksBtn, 0, Qt::AlignHCenter);
    for (TaskUI* task : this->completedTasks) {
        task->setFixedSize(920, 98);
        this->completeTaskslayout->addWidget(task, 0, Qt::AlignHCenter);
    }
    layout->addLayout(this->completeTaskslayout);
    layout->addStretch();

    // --- Навигация ---
    this->navBar = new NavigationBar(this);
    layout->addWidget(this->navBar);

    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, &IndexWnd::showTaskDialog);

    // --- Сигналы от панели навигации ---
    connect(this->navBar, &NavigationBar::switchToCalendar, this, &IndexWnd::switchToCalendar);
    connect(this->navBar, &NavigationBar::switchToFocus, this, &IndexWnd::switchToFocus);
    connect(this->navBar, &NavigationBar::switchToProfile, this, &IndexWnd::switchToProfile);

    // --- Подключение сигналов TaskManager ---
    connect(taskManager, &TaskManager::taskCreated, this, &IndexWnd::onTaskCreated);
    connect(taskManager, &TaskManager::taskUpdated, this, &IndexWnd::onTaskUpdated);
    connect(taskManager, &TaskManager::taskDeleted, this, &IndexWnd::onTaskDeleted);

    // --- Показать уже существующие задачи ---
    for (const Task& task : taskManager->getAllTasks()) {
        this->onTaskCreated(task);
    }
}

IndexWnd::~IndexWnd() {}

void IndexWnd::initDatabase() {
    this->dataBase = new DatabaseManager;
    this->dataBase->initializeDatabase();
}

void IndexWnd::initSearch() {
    this->search = new QLineEdit(this);
    QIcon icon(":/icons/search.png");
    this->search->addAction(icon, QLineEdit::LeadingPosition);
    this->search->setPlaceholderText(tr("Search for your task..."));
    this->search->setStyleSheet("width: 700px; height: 48px; background-color: #292929;");

    connect(this->search, &QLineEdit::textChanged, this, &IndexWnd::searchTaskFilter);
}

void IndexWnd::initSortTags() {
    this->completedTasksBtn = new QPushButton(tr("Completed: "), this);
    this->completedTasksBtn->setStyleSheet("width: 100px; height: 31px; border-radius: 5px; background-color: #292929; margin-top: 10px; margin-bottom: 20px;");

    for (TaskUI* task : this->tasks) {
        if (task->getCompleted())
            this->completedTasks.append(task);
    }
}

void IndexWnd::onDeleteTask(const int row) {
    Task task = this->model->taskAt(row);
    qDebug() << "DATA: " << task.id << ", " << task.title << ", " << task.description;

    // Проверка ID перед удалением
    if (this->model->taskAt(row).id <= 0) {
        qDebug() << "Ошибка: Невалидный ID задачи" << this->model->taskAt(row).id;
        return;
    }

    // dataBase->deleteTaskFromDatabase(this->model);

    // Попытка удалить задачу из базы данных
    if (this->dataBase->deleteTaskFromDatabase(this->model->taskAt(row).id)) {
        qDebug() << "Задача с ID" << this->model->taskAt(row).id << "успешно удалена из базы данных";
        this->model->removeTask(row);  // Убираем задачу из модели
    }
}

void IndexWnd::showTaskDialog() {
    if (this->dialog && this->dialog->isVisible()) {
        this->dialog->raise();
        this->dialog->activateWindow();
        return;
    }

    this->dialog = new TaskDialog(this);
    this->dialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->dialog->setModal(true);

    int dialogHeight = 400;
    int dialogWidth = this->width();

    QPoint startPos(this->x(), this->y() + this->height());
    QPoint endPos(this->x(), this->y() + this->height() - dialogHeight);

    dialog->setGeometry(startPos.x(), startPos.y(), dialogWidth, dialogHeight);
    dialog->show();

    QPropertyAnimation* show = new QPropertyAnimation(this->dialog, "pos");
    show->setDuration(600);
    show->setStartValue(startPos);
    show->setEndValue(endPos);
    show->setEasingCurve(QEasingCurve::OutCubic);
    show->start(QAbstractAnimation::DeleteWhenStopped);

    connect(this->dialog, &TaskDialog::accepted, this, [this]() {
        Task task = this->dialog->getTask();
        this->taskManager->createTask(task);
    });
}

void IndexWnd::updateProfileIcon(const QPixmap& pixmap) {
    this->iconBtn->setIcon(pixmap.scaled(54, 54, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->iconBtn->setIconSize(QSize(54,54));
}

void IndexWnd::searchTaskFilter(const QString &title) {
    QString search = title.trimmed().toLower();

    for (TaskUI* task : this->tasks) {
        QString name = task->getTitle().toLower();

        bool match = name.contains(search);
        task->setVisible(match);
    }
}

void IndexWnd::onTaskCreated(const Task &task) {
    QString formattedDate = task.formatDateTime(task.dueDate);

    if (!formattedDate.contains(tr("Today"))) return;

    TaskUI* taskUI = new TaskUI(task.title, task.description, task.formatDateTime(task.dueDate),
                                task.priority, task.categoryName, task.categoryColor, task.categoryIcon,
                                task.id, task.completed, this);

    // connect(taskUI, &TaskUI::onUpdateTaskToComplete, profile, &ProfileWnd::updateTasksData);

    taskUI->setFixedSize(920, 98);
    if (task.completed) {
        this->completedTasks.append(taskUI);
        this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
    } else
    {
        this->tasks.append(taskUI);
        tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
    }

    connect(taskUI, &TaskUI::onUpdateTaskToComplete, this, [this, taskUI](const int taskId, bool completed) {
        if (completed)
        {
            this->tasks.removeOne(taskUI);
            this->completedTasks.append(taskUI);
            tasksLayout->removeWidget(taskUI);
            this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
        } else
        {
            this->completedTasks.removeOne(taskUI);
            this->tasks.append(taskUI);
            this->completeTaskslayout->removeWidget(taskUI);
            tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
        }
        emit updateTasks();
        this->taskManager->setTaskCompleted(taskId, completed);
    });

    connect(taskUI, &TaskUI::taskClicked, this, [=] {
        TaskInfo* taskInfo = new TaskInfo(task.id, task.title, task.description,
                                          task.formatDateTime(task.dueDate), taskUI, this);
        taskInfo->setFixedSize(500, 600);
        taskInfo->show();

        connect(taskInfo, &TaskInfo::onChangeUI, this, [=] {
            if (!taskUI || !taskInfo) return;

            QString title = taskInfo->getTitle();

            taskUI->setTitle(title);
            taskUI->setCategory(taskInfo->getCategoryName(),
                                taskInfo->getCategoryColor(),
                                taskInfo->getCategoryIcon(), 14, 14);
            taskUI->setPriority(taskInfo->getPriority());
        });
    });
}

void IndexWnd::onTaskUpdated(const Task &task) {
    for (TaskUI* taskUI : this->tasks + this->completedTasks) {
        if (taskUI->getId() == task.id) {
            QString title = task.title;
            taskUI->setTitle(title);
            taskUI->setPriority(QString::number(task.priority));
            taskUI->setCategory(task.categoryName, task.categoryColor, task.categoryIcon, 14, 14);
            taskUI->setCompleted(task.completed);
            taskUI->update();
            break;
        }
    }
}

void IndexWnd::onTaskDeleted(const int taskId) {
    auto removeTask = [this, taskId](QList<TaskUI*>& list, QVBoxLayout* layout) {
        for (TaskUI* taskUI : list) {
            if (taskUI->getId() == taskId) {
                list.removeOne(taskUI);
                layout->removeWidget(taskUI);
                taskUI->deleteLater();
                break;
            }
        }
    };

    removeTask(this->tasks, this->tasksLayout);
    removeTask(this->completedTasks, this->completeTaskslayout);
}
