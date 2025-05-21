#include "taskinfo.h"

TaskInfo::TaskInfo(int id, QString titleStr, QString descStr, QString createData, TaskUI* taskWidget, QWidget* parent)
    : QDialog(parent), Taskid(id), taskUI(taskWidget) {
    // Убираем стандартную рамку и делаем закругленные углы
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);  // Чтобы работали закругления
    this->setFixedSize(700, 450);

    // Основной контейнер с фоном и скруглением
    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(R"(
        #container {
            background-color: #2e2e2e;
            border-radius: 15px;
        }
    )");

    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(20, 20, 20, 20);
    containerLayout->setSpacing(15);

    // Кнопка закрытия
    closeTaskInfo = new QPushButton("×", this);
    closeTaskInfo->setFixedSize(32, 32);
    closeTaskInfo->setStyleSheet(R"(
        QPushButton {
            font-size: 25px;
            color: white;
            background-color: #1D1D1D;
            border-radius: 5px;
            padding-bottom: 5px;
        }
    )");
    connect(closeTaskInfo, &QPushButton::clicked, this, &TaskInfo::close);

    QHBoxLayout* priorityAndTitleLayout = new QHBoxLayout();

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(closeTaskInfo);

    QVBoxLayout* titleLayout = new QVBoxLayout();

    // Элементы задачи
    this->title = new QLabel(titleStr, this);
    this->title->setStyleSheet("font-size: 20px; color: white;");

    titleLayout->addWidget(this->title);

    this->description = new QLabel(descStr, this);
    this->description->setStyleSheet("font-size: 14px; color: #cccccc;");

    titleLayout->addWidget(this->description);

    priorityAndTitleLayout->addLayout(titleLayout);

    QHBoxLayout* timeLayout = new QHBoxLayout();
    timeLayout->setContentsMargins(0,15,0,0);

    QLabel* timeLabel = new QLabel(this);
    QPixmap timeIcon(":/icons/clock.png");
    timeIcon = timeIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    timeLabel->setStyleSheet("border: none");
    timeLabel->setFixedSize(32, 32);
    timeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    timeLabel->setPixmap(timeIcon);

    timeLayout->addWidget(timeLabel);

    QLabel* time = new QLabel("Task Time : ", this);
    time->setStyleSheet("font-size: 17px; color: white;");

    timeLayout->addWidget(time);

    this->taskTime = new QLabel(createData, this);
    this->taskTime->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; max-width: 130px");
    this->taskTime->setAlignment(Qt::AlignCenter);  // Qt-способ центрирования
    timeLayout->addWidget(this->taskTime);

    QPushButton* categoryBtn = new QPushButton(this);
    categoryBtn->setIcon(QIcon(":/icons/category.png"));
    categoryBtn->setIconSize(QSize(32,32));
    categoryBtn->setText("   Task Category : ");
    categoryBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    QPushButton* category = new QPushButton(this);
    category->setIcon(taskUI->getCategoryIcon());
    category->setIconSize(QSize(24,24));
    category->setText(this->taskUI->getCategoryName());
    category->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 40px; min-width: 118px; max-width: 118px");

    QHBoxLayout* categoryLayout = new QHBoxLayout();
    categoryLayout->setContentsMargins(0,15,0,0);
    categoryLayout->addWidget(categoryBtn, 0, Qt::AlignLeft);
    categoryLayout->addWidget(category, 0, Qt::AlignRight);

    QPushButton* priorityBtn = new QPushButton(this);
    priorityBtn->setIcon(QIcon(":/icons/red-flag.png"));
    priorityBtn->setIconSize(QSize(32,32));
    priorityBtn->setText("   Task Category : ");
    priorityBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    QPushButton* priority = new QPushButton(this);
    priority->setText(QString::number(this->taskUI->getPriority()));
    priority->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; min-width: 70px; max-width: 70px");

    QHBoxLayout* priorityLayout = new QHBoxLayout();
    priorityLayout->setContentsMargins(0,15,0,0);
    priorityLayout->addWidget(priorityBtn, 0, Qt::AlignLeft);
    priorityLayout->addWidget(priority, 0, Qt::AlignRight);

    QHBoxLayout* descLayout = new QHBoxLayout();
    descLayout->setContentsMargins(0,15,0,0);

    QPushButton* descBtn = new QPushButton(this);
    descBtn->setIcon(QIcon(":/icons/subtitle.png"));
    descBtn->setIconSize(QSize(32,32));
    descBtn->setText("   Description : ");
    descBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    descLayout->addWidget(descBtn, 0, Qt::AlignLeft);

    this->editDesc = new QPushButton("Add Description", this);
    this->editDesc->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; padding-left: 10px; padding-right: 10px");

    descLayout->addWidget(editDesc, 0, Qt::AlignRight);

    // Нижние кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0,15,0,0);

    this->deleteTask = new QPushButton("   Delete Task", this);
    this->deleteTask->setIcon(QIcon(":/icons/delete.png"));
    this->deleteTask->setIconSize(QSize(32,32));
    this->deleteTask->setStyleSheet("font-size: 17px; color: #E14242; border: none; ");



    connect(this->deleteTask, &QPushButton::clicked, this, &TaskInfo::onDeleteTaskClicked);

    buttonLayout->addWidget(this->deleteTask, 0, Qt::AlignLeft);

    this->editTask = new QPushButton("Edit Task", this);
    this->editTask->setStyleSheet("background-color: #8687E7; font-size: 13px; color: #F8F8FE; height: 48px; margin-top: 60px");

    connect(this->editTask, &QPushButton::clicked, this, [this] {
        this->editTaskWnd = new EditTask(this->title->text(), this->description->text(), this);
        editTaskWnd->show();

        connect(this->editTaskWnd, &EditTask::updateEdit, this, &TaskInfo::onUpdateData);
    });

    // Добавляем всё в layout
    containerLayout->addLayout(topLayout);
    containerLayout->addLayout(priorityAndTitleLayout);
    containerLayout->addLayout(timeLayout);
    containerLayout->addLayout(categoryLayout);
    containerLayout->addLayout(priorityLayout);
    containerLayout->addLayout(descLayout);
    containerLayout->addLayout(buttonLayout);
    containerLayout->addStretch();
    containerLayout->addWidget(this->editTask);

    // Layout всего окна
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(container);
    outerLayout->setContentsMargins(0, 0, 0, 0);
}

QString TaskInfo::getTitle() {
    return this->title->text();
}

QString TaskInfo::getDesc() {
    return this->description->text();
}

void TaskInfo::onDeleteTaskClicked() {
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", this->Taskid);

    if (!query.exec()) {
        qDebug() << "Failed to delete task: " << query.lastError();
        return;
    }

    if (this->taskUI) {
        this->taskUI->deleteLater();
    }

    qDebug() << "Task deleted successfully";

    this->accept();
}

void TaskInfo::onUpdateData() {
    this->title->setText(this->editTaskWnd->text());
    this->description->setText(this->editTaskWnd->getDescription());

    QSqlQuery query;

    query.prepare("UPDATE tasks SET title = :title, description = :description WHERE id = :id");

    query.bindValue(":title", this->title->text());
    query.bindValue(":description", this->description->text());
    query.bindValue(":id", this->Taskid);

    if (!query.exec()) {
        qDebug() << "Failed to update data: " << query.lastError();
        return;
    }

    emit onChangeUI();
}
