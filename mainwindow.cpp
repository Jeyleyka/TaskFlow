#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->dataBase = new DatabaseManager;
    this->dataBase->initializeDatabase();

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    this->table = new QTableView(this);

    this->model = new TaskModel(this);
    this->model->setTasks(this->dataBase->loadTasksFromDatabase());
    this->table->setModel(this->model);

    this->addTaskButton = new QPushButton("Add", this);

    this->deleteTaskButton = new QPushButton("Remove", this);

    layout->addWidget(this->table);
    layout->addWidget(this->addTaskButton);
    central->setLayout(layout);
    this->setCentralWidget(central);

    auto* delegate = new DeleteButtonDelegate(this);
    this->table->setItemDelegateForColumn(5, delegate);

    connect(delegate, &DeleteButtonDelegate::deleteRowRequested, this, &MainWindow::onDeleteTask);


    connect(this->addTaskButton, &QPushButton::clicked, this, [this]() {
        TaskDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted) {
            Task task = dialog.getTask();

            if (this->dataBase->insertTaskToDatabase(task)) {
                qDebug() << "Added task with ID: " << task.id;
                this->model->addTask(task);
            } else {
                QMessageBox::warning(this, "Error", "Failed to save task to database.");
            }
        }
    });

    this->setFixedSize(700,400);
}

MainWindow::~MainWindow() {}

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
