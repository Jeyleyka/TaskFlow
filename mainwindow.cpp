#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager* dataBase = new DatabaseManager;
    dataBase->initializeDatabase();

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    this->table = new QTableView(this);
    this->model = new TaskModel(this);
    this->model->setTasks(dataBase->loadTasksFromDatabase());
    this->table->setModel(this->model);
    this->addTaskButton = new QPushButton("Add", this);

    layout->addWidget(this->table);
    layout->addWidget(this->addTaskButton);
    central->setLayout(layout);
    this->setCentralWidget(central);

    connect(this->addTaskButton, &QPushButton::clicked, this, [this, dataBase]() {
        TaskDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted) {
            Task task = dialog.getTask();

            if (dataBase->insertTaskToDatabase(task)) {
                this->model->addTask(task);
            } else {
                QMessageBox::warning(this, "Error", "Failed to save task to database.");
            }
        }
    });
}

MainWindow::~MainWindow() {}
