#include "calendarwndwidget.h"

CalendarWndWidget::CalendarWndWidget(QWidget* parent)
    : QWidget(parent)
{
    // Основной layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(0);

    // Заголовок страницы
    QLabel* indexTitle = new QLabel("Calendar", this);
    indexTitle->setStyleSheet("font-size: 19px; color: #fff;");

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addStretch();
    titleLayout->addWidget(indexTitle, 0, Qt::AlignCenter);
    titleLayout->addStretch();

    layout->addLayout(titleLayout);

    // Инициализация поиска, сортировки и БД
    this->initDatabase();
    this->initSortTags();

    layout->addWidget(this->btnsContainer, 0, Qt::AlignHCenter);

    // Layout для задач
    this->tasksLayout = new QVBoxLayout;
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0, 30, 0, 0);

    QList<Task> tasks = this->dataBase->loadTasksFromDatabase();
    for (const Task& task : tasks) {
        TaskUI* taskUI = new TaskUI(task.title, task.description, task.formatDateTime(task.dueDate),
                                    task.priority, task.categoryName, task.categoryColor, task.categoryIcon,
                                    task.id, task.completed, this);

        if (taskUI->getDate().contains("Today"))
        {
            taskUI->setFixedSize(920, 98);
            this->tasks.append(taskUI);
            tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
        } else
        {
            taskUI->deleteLater();
            taskUI = nullptr;
        }

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

    this->navBar = new NavigationBar(this);

    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, [this] {
        this->showTaskDialog();
    });

    connect(this->navBar, &NavigationBar::switchToIndex, this, &CalendarWndWidget::switchToindex);

    layout->addLayout(tasksLayout);
    layout->addStretch();
    layout->addWidget(this->navBar);
}

CalendarWndWidget::~CalendarWndWidget() {}

void CalendarWndWidget::initDatabase() {
    this->dataBase = new DatabaseManager;
    this->dataBase->initializeDatabase();
}

void CalendarWndWidget::initSortTags() {
    this->btnsContainer = new QWidget(this);
    this->btnsContainer->setStyleSheet("width: 700px; height: 80px; margin-top: 20px; border-radius: 5px; background-color: #363636");

    this->btnsLayout = new QHBoxLayout(this->btnsContainer);
    this->btnsLayout->setContentsMargins(0,20,0,0);

    this->todayTasksBtn = new QPushButton("Today", this);
    this->todayTasksBtn->setStyleSheet("width: 250px; height: 49px; background-color: #8687E7; margin: 20px;");

    connect(this->todayTasksBtn, &QPushButton::clicked, this, [this] {
        this->completedTasksBtn->setStyleSheet("width: 250px; height: 49px; border: 1px solid #848484; background-color: transparent; margin: 20px;");
        this->todayTasksBtn->setStyleSheet("width: 250px; height: 49px; background-color: #8687E7; margin: 20px;");

        for (TaskUI* task : this->tasks) {
            if (task->getDate().contains("Today"))
                task->show();
            else
                task->hide();
        }
    });

    this->completedTasksBtn = new QPushButton("Completed", this);
    this->completedTasksBtn->setStyleSheet("width: 250px; height: 49px; border: 1px solid #848484; background-color: transparent; margin: 20px;");

    connect(this->completedTasksBtn, &QPushButton::clicked, this, [this] {
        this->todayTasksBtn->setStyleSheet("width: 250px; height: 49px; border: 1px solid #848484; background-color: transparent; margin: 20px;");
        this->completedTasksBtn->setStyleSheet("width: 250px; height: 49px; background-color: #8687E7; margin: 20px;");

        for (TaskUI* task : this->tasks) {
            if (task->getCompleted())
                task->show();
            else
                task->hide();
        }
    });

    this->btnsLayout->addWidget(this->todayTasksBtn);
    this->btnsLayout->addWidget(this->completedTasksBtn);
}

void CalendarWndWidget::onDeleteTask(const int row) {
    Task task = this->model->taskAt(row);
    qDebug() << "DATA: " << task.id << ", " << task.title << ", " << task.description;

    // Проверка ID перед удалением
    if (this->model->taskAt(row).id <= 0) {
        qDebug() << "Ошибка: Невалидный ID задачи" << this->model->taskAt(row).id;
        QMessageBox::warning(this, "Error", "Invalid task ID");
        return;
    }

    // dataBase->deleteTaskFromDatabase(this->model);

    // Попытка удалить задачу из базы данных
    if (this->dataBase->deleteTaskFromDatabase(this->model->taskAt(row).id)) {
        qDebug() << "Задача с ID" << this->model->taskAt(row).id << "успешно удалена из базы данных";
        this->model->removeTask(row);  // Убираем задачу из модели
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove task from database");
    }
}

void CalendarWndWidget::showTaskDialog() {
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
        if (this->dataBase->insertTaskToDatabase(task)) {
            QString formattedDate = task.formatDateTime(task.dueDate);

            auto* taskUI = new TaskUI(task.title, task.description, formattedDate, task.priority, task.categoryName, task.categoryColor, task.categoryIcon, task.id, task.completed, this);
            taskUI->setFixedSize(920, 100);
            this->tasks.append(taskUI);
            this->tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

            connect(taskUI, &TaskUI::taskClicked, this, [=] {
                TaskInfo* taskInfo = new TaskInfo(task.id, task.title, task.description, task.formatDateTime(task.dueDate), taskUI, this);
                taskInfo->setFixedSize(500, 600);
                taskInfo->show();

                connect(taskInfo, &TaskInfo::onChangeUI, this, [this, taskUI, taskInfo] {
                    qDebug() << "Signal onChangeUI received";

                    if (!taskUI || !taskInfo) return;

                    QString title = taskInfo->getTitle();
                    QString desc = taskInfo->getDesc();

                    qDebug() << "Got data:" << title << desc;

                    taskUI->setTitle(title);
                    taskUI->setCategory(taskInfo->getCategoryName(), taskInfo->getCategoryColor(), taskInfo->getCategoryIcon(), 14,14);
                    // taskUI->setDesc(desc);
                });
            });
            // this->model->addTask(task);
        } else {
            QMessageBox::warning(this, "Error", "Failed to save task to database....");
        }

        this->dialog->deleteLater();
        this->dialog = nullptr;
    });

    connect(this->dialog, &TaskDialog::rejected, this, [this]() {
        this->dialog->deleteLater();
        this->dialog = nullptr;
    });
}
