#include "calendarwnd.h"

CalendarWnd::CalendarWnd(TaskManager* taskManager, QWidget* parent)
    : QWidget(parent), taskManager(taskManager), isActive(true), activeButton(nullptr)
{
    if (!this->taskManager) {
        qWarning() << "CalendarWnd: TaskManager is null!";
        return;
    }

    connect(taskManager, &TaskManager::taskCreated, this, &CalendarWnd::onTaskCreated);
    connect(taskManager, &TaskManager::taskUpdated, this, &CalendarWnd::onTaskUpdated);
    connect(taskManager, &TaskManager::taskDeleted, this, &CalendarWnd::onTaskDeleted);

    // Основной layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(0);

    // Заголовок страницы
    QLabel* indexTitle = new QLabel(tr("Calendar"), this);
    indexTitle->setStyleSheet("font-size: 19px; color: #fff;");

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addStretch();
    titleLayout->addWidget(indexTitle, 0, Qt::AlignCenter);
    titleLayout->addStretch();

    layout->addLayout(titleLayout);

    // Инициализация поиска, сортировки и БД
    // this->initDatabase();
    this->initCalendarWidget();

    layout->addWidget(this->calendarWidget, 0, Qt::AlignHCenter);
    this->initSortTags();

    layout->addWidget(this->btnsContainer, 0, Qt::AlignHCenter);

    connect(&ThemeManager::instance(), &ThemeManager::btnChanged, this, [this](const QColor& color) {
        this->updateButtonStyles();
    });

    // Layout для задач
    this->tasksLayout = new QVBoxLayout;
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0, 30, 0, 0);

    QList<Task> tasks = this->taskManager->getAllTasks();

    for (const Task& task : tasks) {
        this->onTaskCreated(task);
    }

    this->navBar = new NavigationBar(this);

    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, [this] {
        this->showTaskDialog();
    });

    connect(this->navBar, &NavigationBar::switchToIndex, this, &CalendarWnd::switchToIndex);
    connect(this->navBar, &NavigationBar::switchToFocus, this, &CalendarWnd::switchToFocus);
    connect(this->navBar, &NavigationBar::switchToProfile, this, &CalendarWnd::switchToProfile);

    layout->addLayout(tasksLayout);
    layout->addStretch();
    layout->addWidget(this->navBar);
}

CalendarWnd::~CalendarWnd() {}

void CalendarWnd::initDatabase() {
    this->dataBase = new DatabaseManager;
    this->dataBase->initializeDatabase();
}

void CalendarWnd::initCalendarWidget() {
    this->calendarWidget = new CalendarWidget(this);

    connect(this->calendarWidget, &CalendarWidget::dateSelected, this, [this](const QDate& date) {
        this->selectedDate = date;
        this->filterTasksByDate(date);

        if (this->selectedDate == QDate::currentDate()) {
            this->currentActiveButton = this->todayTasksBtn;
            this->updateButtonStyles();
        }

        emit onDateUpdated(date);
    });
}

void CalendarWnd::initSortTags() {
    this->btnsContainer = new QWidget(this);
    this->btnsContainer->setStyleSheet("width: 700px; height: 80px; margin-top: 20px; border-radius: 5px; background-color: #363636");

    QColor color = ThemeManager::instance().widgetsColor();
    this->btnsContainer->setStyleSheet("width: 700px; height: 80px; margin-top: 20px; border-radius: 5px; background-color: " + color.name() + ";");

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().widgetsColor();
        this->btnsContainer->setStyleSheet("width: 700px; height: 80px; margin-top: 20px; border-radius: 5px; background-color: " + color.name() + ";");
    });

    connect(&ThemeManager::instance(), &ThemeManager::widgetsChanged, this, [this](const QColor& color) {
        this->btnsContainer->setStyleSheet("width: 700px; height: 80px; margin-top: 20px; border-radius: 5px; background-color: " + color.name());
    });

    this->btnsLayout = new QHBoxLayout(this->btnsContainer);
    this->btnsLayout->setContentsMargins(0,20,0,0);

    this->todayTasksBtn = new QPushButton(tr("Today"), this);
    this->todayTasksBtn->setStyleSheet("width: 250px; height: 49px; background-color: #8182DE; margin: 20px;");

    connect(this->todayTasksBtn, &QPushButton::clicked, this, [this] {
        QDate today = QDate::currentDate();

        this->calendarWidget->setSelectedDate(today);
        this->filterTasksByDate(today);

        this->currentActiveButton = this->todayTasksBtn;
        this->updateButtonStyles();

        for (TaskUI* task : this->tasks) {
            if (task->getRawDueDate() == QDate::currentDate())
                task->show();
            else
                task->hide();
        }

        this->filterTodayOnly = true;
        this->filterCompletedOnly = false;
        // this->applyFilter();
    });

    this->completedTasksBtn = new QPushButton(tr("Completed"), this);
    this->completedTasksBtn->setStyleSheet("width: 250px; height: 49px; border: 1px solid #848484; background-color: transparent; margin: 20px;");

    connect(this->completedTasksBtn, &QPushButton::clicked, this, [this] {
        this->currentActiveButton = this->completedTasksBtn;
        this->updateButtonStyles();

        for (TaskUI* task : this->tasks) {
            if (task->getCompleted())
                task->show();
            else
                task->hide();
        }

        this->filterTodayOnly = false;
        this->filterCompletedOnly = true;
        // this->applyFilter();
    });

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &CalendarWnd::updateButtonStyles);

    this->currentActiveButton = this->todayTasksBtn;
    this->allTaskButtons = { this->todayTasksBtn, this->completedTasksBtn };
    ThemeManager::instance().loadTheme();
    this->updateButtonStyles();

    this->btnsLayout->addWidget(this->todayTasksBtn);
    this->btnsLayout->addWidget(this->completedTasksBtn);
}

void CalendarWnd::onDeleteTask(const int row) {
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

// void CalendarWnd::showEvent(QShowEvent *event) {
//     QWidget::showEvent(event);

//     const QList<Task> tasks = this->taskManager->getAllTasks();
//     this->clearTasksUI();

//     for (const Task& task : tasks)
//         this->onTaskCreated(task);
// }

void CalendarWnd::clearTasksUI() {
    // Удаляем все виджеты из layout
    QLayoutItem* item;
    while ((item = tasksLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr); // отключить от layout
            widget->deleteLater();      // удалить безопасно
        }
        delete item; // удалить сам QLayoutItem
    }
}

void CalendarWnd::showTaskDialog() {
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

    connect(this->dialog, &TaskDialog::rejected, this, [this]() {
        this->dialog->deleteLater();
        this->dialog = nullptr;
    });
}

void CalendarWnd::updateButtonStyles() {
    QColor activeColor = ThemeManager::instance().buttonColor();
    QString activeStyle = QString(
                              "width: 250px; height: 49px; background-color: %1; margin: 20px;"
                              ).arg(activeColor.name());

    QString inactiveStyle =
        "width: 250px; height: 49px; border: 1px solid #848484; background-color: transparent; margin: 20px;";

    for (QPushButton* btn : this->allTaskButtons) {
        btn->setStyleSheet(btn == currentActiveButton ? activeStyle : inactiveStyle);
    }
}

void CalendarWnd::filterTasksByDate(const QDate &date) {
    this->currentActiveButton = nullptr;
    this->updateButtonStyles();

    this->filterTodayOnly = false;
    this->filterCompletedOnly = false;

    for (TaskUI* taskUI : this->tasks) {
        QDate taskDate = taskUI->getRawDueDate();

        if (taskDate == date) {
            taskUI->show();
        } else {
            taskUI->hide();
        }
    }
}

void CalendarWnd::onTaskCreated(const Task &task) {
    QString formattedDate = task.formatDateTime(task.dueDate);

    TaskUI* taskUI = new TaskUI(task.title, task.description, formattedDate,
                                task.priority, task.categoryName, task.categoryColor, task.categoryIcon,
                                task.id, task.completed, this);

    taskUI->setDueDate(task.dueDate.date());

    connect(taskUI, &TaskUI::onUpdateTaskToComplete, this, [this, taskUI, task](const int taskId, bool completed) {
        taskUI->setCompleted(completed);
        this->taskManager->setTaskCompleted(taskId, completed);
    });

    taskUI->setFixedSize(920, 98);
    this->tasks.append(taskUI);
    tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

    if (taskUI->getRawDueDate() != QDate::currentDate()) taskUI->hide();

    connect(taskUI, &TaskUI::taskClicked, this, [=] {
        TaskInfo* taskInfo = new TaskInfo(task.id, task.title, task.description,
                                          formattedDate, taskUI, this);
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

void CalendarWnd::onTaskUpdated(const Task &task) {
    for (int i = 0; i < tasks.size(); ++i) {
        TaskUI* taskUI = tasks[i];
        if (taskUI->getId() == task.id) {
            // Обновляем UI
            QString title = task.title;
            taskUI->setTitle(title);
            taskUI->setPriority(QString::number(task.priority));
            taskUI->setCategory(task.categoryName, task.categoryColor, task.categoryIcon, 14, 14);
            taskUI->setCompleted(task.completed);

            this->taskManager->setTaskCompleted(task.id, task.completed);
            taskUI->update();

            qDebug() << "task: " << task.title << " has completed: " << task.completed;

            // Если задача теперь не соответствует текущему фильтру — удаляем из отображения
            if (filterCompletedOnly && !task.completed) {
                taskUI->hide(); // или delete taskUI;
            } else if (filterTodayOnly) {
                taskUI->show(); // или delete taskUI;
            }

            break;
        }
    }
}

void CalendarWnd::onTaskDeleted(const int taskId) {
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
}
