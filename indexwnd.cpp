#include "indexwnd.h"

IndexWnd::IndexWnd(QWidget* parent)
    : QWidget(parent)
{
    // Основной layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setSpacing(0);

    // Заголовок страницы
    QLabel* indexTitle = new QLabel("Index", this);
    indexTitle->setStyleSheet("font-size: 19px; color: #fff; margin-left: 510px;");

    QPushButton* profileImageBtn = new QPushButton(this);
    profileImageBtn->setIcon(QIcon(":/icons/profile-picture.png"));
    profileImageBtn->setIconSize(QSize(54, 54));
    profileImageBtn->setStyleSheet("width: 54px; height: 54px; margin-right: 480px; border: none");

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addStretch();
    titleLayout->addWidget(indexTitle, 0, Qt::AlignHCenter);
    titleLayout->addStretch(0);
    titleLayout->addWidget(profileImageBtn, 0, Qt::AlignHCenter);
    titleLayout->setContentsMargins(0,20,0,20);

    layout->addLayout(titleLayout);

    // Инициализация поиска, сортировки и БД
    this->initDatabase();
    this->initSearch();

    QVBoxLayout* searchLayout = new QVBoxLayout;
    searchLayout->addWidget(this->search, 0, Qt::AlignHCenter);
    searchLayout->setContentsMargins(0, 0, 0, 20);
    layout->addLayout(searchLayout);

    // Layout для задач
    this->tasksLayout = new QVBoxLayout;
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0, 30, 0, 0);

    this->todayTasksBtn = new QPushButton("Today: ", this);
    this->todayTasksBtn->setStyleSheet("width: 100px; height: 31px; border-radius: 5px; background-color: #292929;");

    tasksLayout->addWidget(this->todayTasksBtn, 0, Qt::AlignHCenter);

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

            connect(taskUI, &TaskUI::onUpdateTaskToComplete, this, [this, taskUI, layout] {
                this->tasks.removeOne(taskUI);
                this->completedTasks.append(taskUI);
                tasksLayout->removeWidget(taskUI);
                this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
            });

            connect(taskUI, &TaskUI::onUpdateTaskToNotComplete, this, [this, taskUI, layout] {
                this->completedTasks.removeOne(taskUI);
                this->tasks.append(taskUI);
                this->completeTaskslayout->removeWidget(taskUI);
                tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
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
        } else {
            taskUI->deleteLater();
            taskUI = nullptr;
        }
    }

    layout->addLayout(tasksLayout);

    this->navBar = new NavigationBar(this);

    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, [this] {
        this->showTaskDialog();
    });

    connect(this->navBar, &NavigationBar::switchToCalendar, this, &IndexWnd::switchToCalendar);
    connect(this->navBar, &NavigationBar::switchToFocus, this, &IndexWnd::switchToFocus);

    this->initSortTags();

    this->completeTaskslayout = new QVBoxLayout;
    this->completeTaskslayout->setSpacing(15);
    this->completeTaskslayout->setContentsMargins(0,20,0,0);

    completeTaskslayout->addWidget(this->completedTasksBtn, 0, Qt::AlignHCenter);

    for (TaskUI* task : this->completedTasks)
    {
        task->setFixedSize(920, 98);
        completeTaskslayout->addWidget(task, 0, Qt::AlignHCenter);
    }

    layout->addLayout(this->completeTaskslayout);
    layout->addStretch();
    layout->addWidget(this->navBar);
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
    this->search->setPlaceholderText("Search for your task...");
    this->search->setStyleSheet("width: 700px; height: 48px; background-color: #292929;");

    connect(this->search, &QLineEdit::textChanged, this, &IndexWnd::searchTaskFilter);
}

void IndexWnd::initSortTags() {
    this->completedTasksBtn = new QPushButton("Completed: ", this);
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
        if (this->dataBase->insertTaskToDatabase(task)) {
            QString formattedDate = task.formatDateTime(task.dueDate);

            auto* taskUI = new TaskUI(task.title, task.description, formattedDate, task.priority, task.categoryName, task.categoryColor, task.categoryIcon, task.id, task.completed, this);
            taskUI->setFixedSize(920, 100);
            this->tasks.append(taskUI);
            this->tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

            connect(taskUI, &TaskUI::onUpdateTaskToComplete, this, [this, taskUI] {
                this->tasks.removeOne(taskUI);
                this->completedTasks.append(taskUI);
                tasksLayout->removeWidget(taskUI);
                this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
            });

            connect(taskUI, &TaskUI::onUpdateTaskToNotComplete, this, [this, taskUI] {
                this->completedTasks.removeOne(taskUI);
                this->tasks.append(taskUI);
                this->completeTaskslayout->removeWidget(taskUI);
                tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
            });

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

void IndexWnd::searchTaskFilter(const QString &title) {
    QString search = title.trimmed().toLower();

    for (TaskUI* task : this->tasks) {
        QString name = task->getTitle().toLower();

        bool match = name.contains(search);
        task->setVisible(match);
    }
}
