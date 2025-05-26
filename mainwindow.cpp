#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->hide(); // скрываем главное окно
    this->setWindowIcon(QIcon(":/icons/logo.png"));

    // Иконка в трее
    this->trayIcon = new QSystemTrayIcon(QIcon(":/icons/logo.png"), this);

    QMenu* menu = new QMenu(this);

    QAction* showAction = menu->addAction("Open");

    connect(showAction, &QAction::triggered, this, &MainWindow::showMaximized);

    QAction* quitAction = menu->addAction("Exit");

    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIcon->setContextMenu(menu);
    trayIcon->setToolTip("Task Flow");
    trayIcon->show();

    this->stackedWidget = new QStackedWidget(this);

    // ---------- Инициализация страниц ----------
    this->calendarWndWidget = new CalendarWnd(this);
    this->calendarWndWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->calendarWndWidget, &CalendarWnd::switchToIndex, this, [this] {
        this->stackedWidget->setCurrentIndex(0);
    });

    connect(this->calendarWndWidget, &CalendarWnd::switchToFocus, this, [this] {
        this->stackedWidget->setCurrentIndex(2);
    });

    this->indexWndWidget = new IndexWnd(this);
    this->indexWndWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->indexWndWidget, &IndexWnd::switchToCalendar, this, [this] {
        this->stackedWidget->setCurrentIndex(1);
    });

    connect(this->indexWndWidget, &IndexWnd::switchToFocus, this, [this] {
        this->stackedWidget->setCurrentIndex(2);
    });

    this->focusWndWidget = new FocusModeWnd(this);
    this->focusWndWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->focusWndWidget, &FocusModeWnd::switchToIndex, this, [this] {
        this->stackedWidget->setCurrentIndex(0);
    });

    connect(this->focusWndWidget, &FocusModeWnd::switchToCalendar, this, [this] {
        this->stackedWidget->setCurrentIndex(1);
    });

    // ---------- Добавление страниц ----------
    this->stackedWidget->addWidget(this->indexWndWidget);
    this->stackedWidget->addWidget(this->calendarWndWidget);
    this->stackedWidget->addWidget(this->focusWndWidget);
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

void MainWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    this->hide();

    if (trayIcon) {
        this->trayIcon->showMessage("Working in background", "App folded in tray", QSystemTrayIcon::Information, 3000);
    }
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        this->showNormal();
        this->activateWindow();
        this->raise();
    }
}

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
