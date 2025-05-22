#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget* central = new QWidget(this);
    this->layout = new QVBoxLayout(central);

    this->initDatabase();
    // this->initModel();
    // this->initTable();
    this->initSearch();
    this->initSortTags();
    this->initNavigationBar();

    // layout->addWidget(this->table);
    QList<Task> tasks = this->dataBase->loadTasksFromDatabase();
    this->tasksLayout = new QVBoxLayout;
    // this->tasksLayout->addStretch(0);
    this->tasksLayout->setSpacing(15);
    this->tasksLayout->setContentsMargins(0,30,0,0);

    for (const Task& task: tasks) {

        qDebug() << "COMPLETED: " << task.completed;
        TaskUI* taskUI = new TaskUI(task.title, task.description, task.formatDateTime(task.dueDate), task.priority, task.categoryName, task.categoryColor, task.categoryIcon, task.id, task.completed, this);
        taskUI->setFixedSize(920, 98);
        this->tasks.append(taskUI);
        this->tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

        qDebug() << "current date: " << QDate::currentDate();
        qDebug() << "task date: " << taskUI->getCategoryDate();

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
                taskUI->setPriority(taskInfo->getPriority());
                // taskUI->setDesc(desc);
            });
        });
    }

    this->indexTitle = new QLabel("Index", this);
    this->indexTitle->setStyleSheet("font-size: 19px; color: #fff;");

    this->profileImageBtn = new QPushButton(this);
    this->profileImageBtn->setIcon(QIcon(":/icons/profile-picture.png"));
    this->profileImageBtn->setIconSize(QSize(42,42));
    this->profileImageBtn->setStyleSheet("width: 42px; height: 42px; border: none");

    auto* titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(0, 0, 20, 20);
    titleLayout->setSpacing(10); // небольшой отступ между элементами

    // Прокладка слева
    titleLayout->addStretch();

    // Заголовок по центру
    titleLayout->addWidget(this->indexTitle, 0, Qt::AlignCenter);

    // Прокладка между заголовком и иконкой
    titleLayout->addStretch();

    // Иконка справа
    titleLayout->addWidget(this->profileImageBtn, 0, Qt::AlignRight);

    auto* searchLayout = new QVBoxLayout;
    searchLayout->addWidget(this->search, 0, Qt::AlignHCenter);
    searchLayout->setContentsMargins(0,0,0,20);

    layout->addLayout(titleLayout);
    layout->addLayout(searchLayout);
    layout->addWidget(this->sortTags, 0, Qt::AlignHCenter);
    layout->setAlignment(Qt::AlignHCenter);
    layout->addLayout(tasksLayout);
    layout->addStretch();

    auto* navigationLayout = new QVBoxLayout;

    navigationLayout->addWidget(this->navigationBar);
    layout->setContentsMargins(0, 20, 0, 0);  // Убирает отступы со всех сторон
    layout->setSpacing(0);

    layout->addLayout(navigationLayout);
    central->setLayout(layout);
    this->setCentralWidget(central);

    auto* delegate = new DeleteButtonDelegate(this);

    connect(delegate, &DeleteButtonDelegate::deleteRowRequested, this, &MainWindow::onDeleteTask);

    // this->table->setItemDelegateForColumn(5, delegate);
    this->showMaximized();
}

MainWindow::~MainWindow() {}

void MainWindow::initTable() {
    this->table = new QTableView(this);
    this->table->setModel(this->model);
}
void MainWindow::initModel() {
    this->model = new TaskModel(this);
    this->model->setTasks(this->dataBase->loadTasksFromDatabase());
}

void MainWindow::initDatabase() {
    this->dataBase = new DatabaseManager;
    this->dataBase->initializeDatabase();
}

void MainWindow::initSearch() {
    this->search = new QLineEdit(this);
    QIcon icon(":/icons/search.png");
    this->search->addAction(icon, QLineEdit::LeadingPosition);
    this->search->setPlaceholderText("Search for your task...");
    this->search->setStyleSheet("width: 700px; height: 48px; background-color: #292929;");

    connect(this->search, &QLineEdit::textChanged, this, &MainWindow::searchTaskFilter);
}

void MainWindow::initSortTags() {
    this->sortTags = new QComboBox(this);
    this->sortTags->addItem("All");
    this->sortTags->addItem("Today");
    this->sortTags->addItem("Completed");

    connect(this->sortTags, &QComboBox::currentTextChanged, this, &MainWindow::filterTasks);

    this->sortTags->setStyleSheet(R"(
    QComboBox {
        background-color: #363636;
        color: white;
        padding: 5px;
        border-radius: 5px;
        max-width: 100px;
        max-height: 31px;
    }

    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: top right;
        width: 30px; /* ширина области со стрелкой */
        border-top-right-radius: 5px;
        border-bottom-right-radius: 5px;
        background-color: #363636; /* тот же цвет, что и у основного поля */
    }

    QComboBox::down-arrow {
        image: url(:/icons/down-arrow.png);
        padding-top: 5px;
        width: 18px;
        height: 18px;
    }

    QComboBox QAbstractItemView {
        background-color: #3a3a3a;
        selection-background-color: #8687E7;
    }

    QScrollBar:vertical {
        border: none;
        background: #3a3a3a;
        width: 6px;             /* ШИРИНА скроллбара */
        margin: 0px 0px 0px 0px;
    }

    QScrollBar::handle:vertical {
        background: #8687E7;     /* Цвет "ползунка" */
        min-height: 20px;
        border-radius: 3px;
    }

    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0px;
    }

    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
        background: none;
    }
)");
}

void MainWindow::initNavigationBar() {
    this->navigationBar = new QWidget(this);
    this->navigationBar->setStyleSheet("background-color: #363636;");
    this->navigationBar->setMinimumWidth(0);
    this->navigationBar->setFixedHeight(90);
    this->navigationBar->setMaximumWidth(QWIDGETSIZE_MAX);
    this->navigationBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->indexBtn = new QPushButton(this->navigationBar);
    this->indexBtn->setIcon(QIcon(":/icons/home.png"));
    this->indexBtn->setIconSize(QSize(26,26));
    this->indexBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    this->indexLabel = new QLabel("Index", this->navigationBar);
    this->indexLabel->setStyleSheet("font-size: 14px;");

    auto* indexLayout = new QVBoxLayout();
    indexLayout->addWidget(this->indexBtn);
    indexLayout->addWidget(this->indexLabel);
    indexLayout->setAlignment(Qt::AlignCenter);
    indexLayout->setContentsMargins(0,15,0,0);

    this->calendarBtn = new QPushButton(this->navigationBar);
    this->calendarBtn->setIcon(QIcon(":/icons/calendar.png"));
    this->calendarBtn->setIconSize(QSize(26,26));
    this->calendarBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    this->calendarLabel = new QLabel("Calendar", this->navigationBar);
    this->calendarLabel->setStyleSheet("font-size: 14px;");

    auto* calendarLayout = new QVBoxLayout();
    calendarLayout->addWidget(this->calendarBtn);
    calendarLayout->addWidget(this->calendarLabel);
    calendarLayout->setAlignment(Qt::AlignCenter);
    calendarLayout->setContentsMargins(20, 15, 30, 0);

    this->addTaskButton = new QPushButton(this->navigationBar);
    this->addTaskButton->setIcon(QIcon(":/icons/plus.png"));
    this->addTaskButton->setIconSize(QSize(30,30));
    this->addTaskButton->setStyleSheet("width: 64px; height: 64px; background-color: #8687E7; border-radius: 32px;");

    connect(this->addTaskButton, &QPushButton::clicked, this, &MainWindow::showTaskDialog);

    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(this->addTaskButton);

    this->focusBtn = new QPushButton(this->navigationBar);
    this->focusBtn->setIcon(QIcon(":/icons/clock.png"));
    this->focusBtn->setIconSize(QSize(35,35));
    this->focusBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    this->focusLabel = new QLabel("Focuse", this->navigationBar);
    this->focusLabel->setStyleSheet("font-size: 14px;");

    auto* focusLayout = new QVBoxLayout();
    focusLayout->addWidget(this->focusBtn);
    focusLayout->addWidget(this->focusLabel);
    focusLayout->setAlignment(Qt::AlignCenter);
    focusLayout->setContentsMargins(30,15,20,0);

    this->profileBtn = new QPushButton(this->navigationBar);
    this->profileBtn->setIcon(QIcon(":/icons/human.png"));
    this->profileBtn->setIconSize(QSize(32,32));
    this->profileBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    this->profileLabel = new QLabel("Profile", this->navigationBar);
    this->profileLabel->setStyleSheet("font-size: 14px;");

    auto* profileLayout = new QVBoxLayout();
    profileLayout->addWidget(this->profileBtn);
    profileLayout->addWidget(this->profileLabel);
    profileLayout->setAlignment(Qt::AlignCenter);
    profileLayout->setContentsMargins(0, 15, 0, 0);

    auto* mainLayout = new QHBoxLayout(this->navigationBar);
    mainLayout->addLayout(indexLayout);
    mainLayout->addLayout(calendarLayout);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(focusLayout);
    mainLayout->addLayout(profileLayout);
    mainLayout->setAlignment(Qt::AlignCenter);
}

void MainWindow::onDeleteTask(const int row) {
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

void MainWindow::showTaskDialog() {
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

void MainWindow::filterTasks(const QString& filter) {
    for (TaskUI* task : this->tasks) {
        if (filter == "All") {
            task->show();
        }
        else if (filter == "Today") {
            if (task->getCategoryDate().contains("Today"))
            {
                task->show();
            } else
                task->hide();
        }
        else if (filter == "Completed") {
            if (task->getCompleted())
                task->show();
            else
                task->hide();
        }

    }
}

void MainWindow::searchTaskFilter(const QString &title) {
    QString search = title.trimmed().toLower();

    for (TaskUI* task : this->tasks) {
        QString name = task->getTitle().toLower();

        bool match = name.contains(search);
        task->setVisible(match);
    }
}
