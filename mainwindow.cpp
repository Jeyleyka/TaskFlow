#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->stackedWidget = new QStackedWidget(this);

    // ---------- Инициализация страниц ----------
    this->calendarWndWidget = new CalendarWndWidget(this);
    this->calendarWndWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->calendarWndWidget, &CalendarWndWidget::switchToindex, this, [this] {
        this->stackedWidget->setCurrentIndex(0);
    });

    this->indexWndWidget = new IndexWndWidget(this);
    this->indexWndWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->indexWndWidget, &IndexWndWidget::switchToCalendar, this, [this] {
        this->stackedWidget->setCurrentIndex(1);
    });

    // ---------- Добавление страниц ----------
    this->stackedWidget->addWidget(this->indexWndWidget);
    this->stackedWidget->addWidget(this->calendarWndWidget);
    this->stackedWidget->setCurrentIndex(0);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(this->stackedWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* container = new QWidget(this);
    container->setLayout(mainLayout);

    this->setCentralWidget(container);

    this->showMaximized();
}

MainWindow::~MainWindow() {}

// void MainWindow::onDeleteTask(const int row) {
//     // Проверка ID перед удалением
//     if (this->model->taskAt(row).id <= 0) {
//         qDebug() << "Ошибка: Невалидный ID задачи" << this->model->taskAt(row).id;
//         QMessageBox::warning(this, "Error", "Invalid task ID");
//         return;
//     }

//     // dataBase->deleteTaskFromDatabase(this->model);

//     // Попытка удалить задачу из базы данных
//     if (this->dataBase->deleteTaskFromDatabase(this->model->taskAt(row).id)) {
//         qDebug() << "Задача с ID" << this->model->taskAt(row).id << "успешно удалена из базы данных";
//         this->model->removeTask(row);  // Убираем задачу из модели
//     } else {
//         QMessageBox::warning(this, "Error", "Failed to remove task from database");
//     }
// }
