#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    this->initDatabase();
    this->initModel();
    this->initTable();
    this->initAddTaskBtn();

    layout->addWidget(this->table);
    layout->addWidget(this->addTaskButton);
    central->setLayout(layout);
    this->setCentralWidget(central);

    auto* delegate = new DeleteButtonDelegate(this);

    connect(delegate, &DeleteButtonDelegate::deleteRowRequested, this, &MainWindow::onDeleteTask);

    this->table->setItemDelegateForColumn(5, delegate);
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
    this->dialog->setModal(false);

    int dialogHeight = 400;
    int dialogWidth = this->width();

    QPoint startPos(this->x(), this->y() + this->height());
    QPoint endPos(this->x(), this->y() + this->height() - dialogHeight);

    dialog->setGeometry(this->x(), this->y() + this->height(), dialogWidth, dialogHeight);
    dialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog->show();

    QPropertyAnimation* show = new QPropertyAnimation(this->dialog, "pos");
    show->setDuration(600);
    show->setStartValue(startPos);
    show->setEndValue(endPos);
    show->setEasingCurve(QEasingCurve::OutCubic);
    show->start(QAbstractAnimation::DeleteWhenStopped);

    connect(this->dialog, &TaskDialog::finished, this, [this]() {
        this->dialog->deleteLater();
        this->dialog = nullptr;
    });

    if (dialog->exec() == QDialog::Accepted) {
        Task task = dialog->getTask();

        if (this->dataBase->insertTaskToDatabase(task)) {
            qDebug() << "Added task with ID: " << task.id;
            this->model->addTask(task);
        } else {
            QMessageBox::warning(this, "Error", "Failed to save task to database.");
        }
    }
}
