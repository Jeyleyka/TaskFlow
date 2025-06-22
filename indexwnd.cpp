#include "indexwnd.h"

IndexWnd::IndexWnd(TaskManager* taskManager, QWidget* parent)
    : QWidget(parent), taskManager(taskManager)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(0);

    ThemeManager::instance().loadTheme();

    // --- Заголовок и иконка профиля ---
    QLabel* indexTitle = new QLabel(tr("Index"), this);
    indexTitle->setStyleSheet("font-size: 19px; margin-left: 310px; padding-right: 250px; color: #fff;");
    indexTitle->setAlignment(Qt::AlignCenter);

    this->iconBtn = nullptr; // обязательно обнуляем
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

    QHBoxLayout* wrapperLayout = new QHBoxLayout();
    wrapperLayout->setContentsMargins(0, 0, 0, 20);
    wrapperLayout->addStretch();
    wrapperLayout->addWidget(indexTitle);
    wrapperLayout->addSpacing(10);
    if (this->iconBtn) wrapperLayout->addWidget(this->iconBtn);
    wrapperLayout->addStretch();

    QWidget* wrapperWidget = new QWidget(this);
    wrapperWidget->setLayout(wrapperLayout);
    layout->addWidget(wrapperWidget);

    // --- Поиск ---
    this->initSearch();
    QVBoxLayout* searchLayout = new QVBoxLayout;
    searchLayout->addWidget(this->search, 0, Qt::AlignHCenter);
    searchLayout->setContentsMargins(0, 0, 0, 20);
    layout->addLayout(searchLayout);

    // --- Активные задачи с прокруткой ---
    this->todayTasksBtn = new QPushButton(tr("Today:"), this);
    this->todayTasksBtn->setStyleSheet("width: 100px; height: 31px; border-radius: 5px; background-color: #292929;");

    QWidget* scrollContent = new QWidget();
    this->tasksLayout = new QVBoxLayout(scrollContent);
    this->tasksLayout->setAlignment(Qt::AlignTop);
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0, 30, 0, 0);
    this->tasksLayout->addWidget(this->todayTasksBtn, 0, Qt::AlignHCenter);

    scrollContent->setLayout(this->tasksLayout);

    this->scrollArea = new QScrollArea(this);
    this->scrollArea->setWidgetResizable(true);
    this->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scrollArea->setFrameShape(QFrame::NoFrame);
    this->scrollArea->setWidget(scrollContent);

    layout->addWidget(this->scrollArea);

    // --- Выполненные задачи с прокруткой ---
    this->completedTasksBtn = new QPushButton(tr("Completed:"), this);
    this->completedTasksBtn->setStyleSheet("width: 100px; height: 31px; border-radius: 5px; background-color: #292929;");

    QWidget* completedContent = new QWidget();
    this->completeTaskslayout = new QVBoxLayout(completedContent);
    this->completeTaskslayout->setAlignment(Qt::AlignTop);
    this->completeTaskslayout->setSpacing(15);
    this->completeTaskslayout->setContentsMargins(0, 20, 0, 0);
    this->completeTaskslayout->addWidget(this->completedTasksBtn, 0, Qt::AlignHCenter);

    completedContent->setLayout(this->completeTaskslayout);

    this->completedScroll = new QScrollArea(this);
    this->completedScroll->setWidgetResizable(true);
    this->completedScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->completedScroll->setFrameShape(QFrame::NoFrame);
    this->completedScroll->setWidget(completedContent);

    layout->addWidget(this->completedScroll);

    // --- Навигация ---
    this->navBar = new NavigationBar(this);
    layout->addWidget(this->navBar);

    this->profile = new ProfileWnd;
    this->initSortTags();

    // --- Сигналы от панели навигации ---
    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, &IndexWnd::showTaskDialog);
    connect(this->navBar, &NavigationBar::switchToCalendar, this, &IndexWnd::switchToCalendar);
    connect(this->navBar, &NavigationBar::switchToFocus, this, &IndexWnd::switchToFocus);
    connect(this->navBar, &NavigationBar::switchToProfile, this, &IndexWnd::switchToProfile);

    // --- Сигналы от TaskManager ---
    connect(taskManager, &TaskManager::taskCreated, this, &IndexWnd::onTaskCreated);
    connect(taskManager, &TaskManager::taskUpdated, this, &IndexWnd::onTaskUpdated);
    connect(taskManager, &TaskManager::taskDeleted, this, &IndexWnd::onTaskDeleted);

    // --- Добавляем существующие задачи ---
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

void IndexWnd::showTaskDialog() {
    if (this->addTaskWnd && this->addTaskWnd->isVisible()) {
        this->addTaskWnd->raise();
        this->addTaskWnd->activateWindow();
        return;
    }

    this->addTaskWnd = new AddTask(this);
    this->addTaskWnd->showCentered(this);
    // this->dialog->activateWindow();
    // this->dialog->setFocus();
    // this->dialog->setModal(true);

    connect(this->addTaskWnd, &AddTask::onCreateTask, this, [this]() {
        Task task = this->addTaskWnd->getTask();
        this->taskManager->createTask(task);
        this->addTaskWnd->deleteLater();
        this->addTaskWnd = nullptr;
    });

    connect(this->addTaskWnd, &AddTask::onCloseWnd, this, [this]() {
        this->addTaskWnd->deleteLater();
        this->addTaskWnd = nullptr;
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
    qDebug() << "time: " << task.dueDate.toString();
    QString formattedDate = task.formatDateTime(task.creationDate);

    if (!formattedDate.contains(tr("Today"))) return;

    TaskUI* taskUI = new TaskUI(task.title, task.description, formattedDate,
                                task.priority, task.categoryName, task.categoryColor, task.categoryIcon,
                                task.id, task.completed, this);

    taskUI->setDueDate(task.dueDate.date());

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
        if (completed) {
            this->tasks.removeOne(taskUI);
            this->completedTasks.append(taskUI);
            tasksLayout->removeWidget(taskUI);
            this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
        } else {
            this->completedTasks.removeOne(taskUI);
            this->tasks.append(taskUI);
            this->completeTaskslayout->removeWidget(taskUI);
            tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
        }

        emit updateTasks(this->taskManager->getTaskById(taskId));
        this->taskManager->setTaskCompleted(taskId, completed);
    });

    QPointer<TaskUI> safeTaskUI = taskUI;

    connect(taskUI, &TaskUI::taskClicked, this, [=] {
        Task newTask = this->dataBase->getTaskById(task.id);

        TaskInfo* taskInfo = new TaskInfo(newTask.id, newTask.title, newTask.description,
                                          newTask.dueDate, safeTaskUI, this);

        QPointer<TaskInfo> safeTaskInfo = taskInfo;

        taskInfo->setFixedSize(500, 600);
        taskInfo->show();

        connect(taskInfo, &QDialog::finished, taskInfo, &QObject::deleteLater);

        connect(taskInfo, &TaskInfo::taskDeleted, this, [this, taskUI] {
            this->taskManager->deleteTask(taskUI->getId());
            taskUI->deleteLater();
        });

        connect(taskInfo, &TaskInfo::onChangeUI, this, [=] {
            if (!safeTaskUI || !safeTaskInfo) return;

            Task updatedTask = this->dataBase->getTaskById(task.id);

            safeTaskInfo->loadTaskData(updatedTask);

            safeTaskUI->setTitle(updatedTask.title);
            safeTaskUI->setDueDate(updatedTask.dueDate.date());
            safeTaskUI->setPriority(QString::number(updatedTask.priority));

            safeTaskUI->setCategory(updatedTask.categoryName, updatedTask.categoryColor, updatedTask.categoryIcon, 14, 14);

            this->taskManager->updateTask(updatedTask);
        });
    });
}

void IndexWnd::onTaskUpdated(const Task &task) {
    TaskUI* taskUI = nullptr;

    // Ищем TaskUI с этим id в обоих списках
    for (TaskUI* t : tasks) {
        if (t->getId() == task.id) {
            taskUI = t;
            break;
        }
    }
    if (!taskUI) {
        for (TaskUI* t : completedTasks) {
            if (t->getId() == task.id) {
                taskUI = t;
                break;
            }
        }
    }
    if (!taskUI) return; // не нашли, может нужно создать

    // Обновляем UI
    QString title = task.title;

    taskUI->setTitle(title);
    taskUI->setCategory(task.categoryName, task.categoryColor, task.categoryIcon, 14, 14);
    taskUI->setPriority(QString::number(task.priority));
    taskUI->setCompleted(task.completed);

    // Перемещаем в нужный список и layout если изменился статус completed
    if (task.completed) {
        if (!completedTasks.contains(taskUI)) {
            tasks.removeOne(taskUI);
            tasksLayout->removeWidget(taskUI);

            completedTasks.append(taskUI);
            completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
        }
    } else {
        if (!tasks.contains(taskUI)) {
            completedTasks.removeOne(taskUI);
            completeTaskslayout->removeWidget(taskUI);

            tasks.append(taskUI);
            tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
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
