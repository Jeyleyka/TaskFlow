#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget* central = new QWidget(this);
    this->layout = new QVBoxLayout(central);

    this->initDatabase();
    // this->initModel();
    // this->initTable();
    this->initAddTaskBtn();

    // layout->addWidget(this->table);
    QList<Task> tasks = this->dataBase->loadTasksFromDatabase();

    for (const Task& task: tasks) {
        QLabel* priority = new QLabel();

        if (task.priority == "High")
            priority->setText(":/icons/red-circle.png");
        else if (task.priority == "Medium")
            priority->setText(":/icons/yellow-circle.png");
        else
            priority->setText(":/icons/blue-circle.png");

        TaskUI* taskUI = new TaskUI(task.title, task.description, task.formatDateTime(task.dueDate), priority->text(), this);
        taskUI->setFixedSize(700,100);
        this->layout->addWidget(taskUI);
    }

    layout->addWidget(this->addTaskButton);


    // TaskUI* newTask = new TaskUI(this);
    // newTask->setFixedSize(700, 300);

    // layout->addWidget(newTask);

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

void MainWindow::initAddTaskBtn() {
    this->addTaskButton = new QPushButton("Add", this);

    connect(this->addTaskButton, &QPushButton::clicked, this, &MainWindow::showTaskDialog);
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
            QLabel* priority = new QLabel();

            if (task.priority == "High")
                priority->setText(":/icons/red-circle.png");
            else if (task.priority == "Medium")
                priority->setText(":/icons/yellow-circle.png");
            else
                priority->setText(":/icons/blue-circle.png");

            auto* taskUI = new TaskUI(task.title, task.description, formattedDate, priority->text());
            taskUI->setFixedSize(700, 300);
            this->layout->addWidget(taskUI);
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

