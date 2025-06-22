#include "taskinfo.h"

TaskInfo::TaskInfo(int id, QString titleStr, QString descStr,  const QDateTime& dueDateTime, TaskUI* taskWidget, QWidget* parent)
    : QDialog(parent), Taskid(id), taskUI(taskWidget), dateTime(dueDateTime) {
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

    QPushButton* circle = new QPushButton(this);
    circle->setIcon(QIcon(":/icons/info-circle.svg"));
    circle->setIconSize(QSize(16,16));
    circle->setStyleSheet("background-color: transparent; border: none");
    circle->setFixedSize(16,16);

    priorityAndTitleLayout->addWidget(circle);


    QVBoxLayout* titleLayout = new QVBoxLayout();

    // Элементы задачи
    this->title = new QLabel(titleStr, this);
    this->title->setStyleSheet("font-size: 20px; color: white;");

    titleLayout->addWidget(this->title);

    this->description = new QLabel(descStr, this);
    this->description->setStyleSheet("font-size: 14px; color: #cccccc;");

    titleLayout->addWidget(this->description);

    priorityAndTitleLayout->addLayout(titleLayout);

    QPushButton* edit = new QPushButton(this);
    edit->setIcon(QIcon(":/icons/edit-info.svg"));
    edit->setIconSize(QSize(24,24));
    edit->setStyleSheet("background-color: transparent; border: none");
    edit->setFixedSize(24,24);

    connect(edit, &QPushButton::clicked, this, [this] {
        this->editTaskWnd = new EditTask(this->title->text(), this->description->text(), this);
        editTaskWnd->show();

        connect(this->editTaskWnd, &EditTask::updateEdit, this, [this] {
            this->title->setText(this->editTaskWnd->getTitle());
            this->description->setText(this->editTaskWnd->getDescription());
        });
    });

    priorityAndTitleLayout->addWidget(edit);

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

    QLabel* time = new QLabel(tr("Task Time: "), this);
    time->setStyleSheet("font-size: 17px; color: white;");

    timeLayout->addWidget(time);

    this->taskTime = new QPushButton(dueDateTime.toString("dd.MM.yyyy 'at' hh:mm"), this);

    this->taskTime->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; "
                                  "max-width: 130px");

    connect(this->taskTime, &QPushButton::clicked, this, [this] {
        CustomCalendar* calendar = new CustomCalendar();
        calendar->showCentered(this);

        connect(calendar, &CustomCalendar::dateSelected, this, [this](const QDate& date, int hour, int minutes, bool isAm) {
            this->date = date;
            this->hours = hour;
            this->minutes = minutes;
            this->isAm = isAm;

            if (minutes < 10)
                this->minutesStr = "0" + QString::number(minutes);
            else
                this->minutesStr = QString::number(minutes);

            this->dateTime = QDateTime(date, QTime(hour, minutes));

            this->taskTime->setText(date.toString("dd.MM.yyyy") + " at " + QString::number(hour) + ":" + this->minutesStr);
        });
    });

    timeLayout->addWidget(this->taskTime);

    QPushButton* categoryBtn = new QPushButton(this);
    categoryBtn->setIcon(QIcon(":/icons/category.png"));
    categoryBtn->setIconSize(QSize(32,32));
    categoryBtn->setText(tr("   Task Category: "));
    categoryBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    this->category = new QPushButton(this);
    this->category->setIcon(taskUI->getCategoryIcon());
    this->category->setIconSize(QSize(24,24));
    this->category->setText(this->taskUI->getCategoryName());
    this->category->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; height: 40px; min-width: 118px; max-width: 118px; "
                                  "background-color: " + this->taskUI->getCategoryColor().name());

    this->categoryColor = this->taskUI->getCategoryColor().name();

    connect(this->category, &QPushButton::clicked, this, [this] {
        ChooseCategory* chooseCat = new ChooseCategory(this);
        chooseCat->show();

        connect(chooseCat, &ChooseCategory::categorySelected, this, [this](const QString& name, const QColor& color, const QIcon& icon) {
            this->category->setIcon(icon);
            this->category->setIconSize(QSize(24,24));
            this->category->setText(name);
            this->category->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; height: 40px; min-width: 118px; max-width: 118px; "
                                          "background-color: " + color.name());

            this->categoryColor = color;
        });
    });

    QHBoxLayout* categoryLayout = new QHBoxLayout();
    categoryLayout->setContentsMargins(0,15,0,0);
    categoryLayout->addWidget(categoryBtn, 0, Qt::AlignLeft);
    categoryLayout->addWidget(category, 0, Qt::AlignRight);

    QPushButton* priorityBtn = new QPushButton(this);
    priorityBtn->setIcon(QIcon(":/icons/red-flag.png"));
    priorityBtn->setIconSize(QSize(32,32));
    priorityBtn->setText(tr("   Task Priority: "));
    priorityBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    this->priority = new QPushButton(this);
    this->priority->setText(QString::number(this->taskUI->getPriority()));
    this->priority->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; "
                                  "min-width: 70px; max-width: 70px");

    connect(this->priority, &QPushButton::clicked, this, [this] {
        ChoosePriority* choosePrior = new ChoosePriority(this);
        choosePrior->show();

        connect(choosePrior, &ChoosePriority::prioritySelected, this, [this](const int& priority) {
            this->priority->setText(QString::number(priority));
            this->priority->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; "
                                          "min-width: 70px; max-width: 70px");
        });
    });

    QHBoxLayout* priorityLayout = new QHBoxLayout();
    priorityLayout->setContentsMargins(0,15,0,0);
    priorityLayout->addWidget(priorityBtn, 0, Qt::AlignLeft);
    priorityLayout->addWidget(priority, 0, Qt::AlignRight);

    QHBoxLayout* descLayout = new QHBoxLayout();
    descLayout->setContentsMargins(0,15,0,0);

    QPushButton* descBtn = new QPushButton(this);
    descBtn->setIcon(QIcon(":/icons/subtitle.png"));
    descBtn->setIconSize(QSize(32,32));
    descBtn->setText(tr("   Description : "));
    descBtn->setStyleSheet("font-size: 17px; color: #fff; border: none;");

    descLayout->addWidget(descBtn, 0, Qt::AlignLeft);

    this->editDesc = new QPushButton(tr("Add Description"), this);
    this->editDesc->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; background-color: #444444; height: 37px; padding-left: 10px; padding-right: 10px");

    descLayout->addWidget(editDesc, 0, Qt::AlignRight);

    // Нижние кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0,15,0,0);

    this->deleteTask = new QPushButton(tr("   Delete Task"), this);
    this->deleteTask->setIcon(QIcon(":/icons/delete.png"));
    this->deleteTask->setIconSize(QSize(32,32));
    this->deleteTask->setStyleSheet("font-size: 17px; color: #E14242; border: none; ");

    connect(this->deleteTask, &QPushButton::clicked, this, [this] {
        DeleteTaskWnd* deleteWnd = new DeleteTaskWnd(this->title->text(), this->Taskid, this);
        deleteWnd->show();

        connect(deleteWnd, &DeleteTaskWnd::deleteTask, this, &TaskInfo::onDeleteTaskClicked);
    });

    buttonLayout->addWidget(this->deleteTask, 0, Qt::AlignLeft);

    this->editTask = new QPushButton(tr("Edit Task"), this);
    this->editTask->setStyleSheet("background-color: #8687E7; font-size: 13px; color: #F8F8FE; height: 48px; margin-top: 60px");

    connect(this->editTask, &QPushButton::clicked, this, &TaskInfo::onUpdateData);

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

const QString TaskInfo::getTitle() {
    return this->title->text();
}

const QString TaskInfo::getDesc() {
    return this->description->text();
}

const QDateTime TaskInfo::getDueDate() {
    return this->dateTime;
}

const QString TaskInfo::getCategoryName() {
    return this->category->text();
}

const QColor TaskInfo::getCategoryColor() {
    return this->categoryColor;
}

const QIcon TaskInfo::getCategoryIcon() {
    return this->category->icon();
}

const QString TaskInfo::getPriority() {
    return this->priority->text();
}

Task TaskInfo::getUpdatedTask() const {
    Task task;
    task.id = this->Taskid;
    task.title = this->title->text();
    task.description = this->description->text();
    task.dueDate = QDateTime(this->date, QTime(this->hours, this->minutes));
    task.priority = this->priority->text().toInt();
    task.categoryName = this->category->text();
    task.categoryColor = this->categoryColor;
    task.categoryIcon = this->category->icon();
    return task;
}

void TaskInfo::onDeleteTaskClicked() {
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", this->Taskid);

    if (!query.exec()) {
        qDebug() << "Failed to delete task: " << query.lastError();
        return;
    }

    this->taskUI->disconnect();
    this->taskUI->deleteLater();
    this->taskUI = nullptr;

    qDebug() << "Task deleted successfully";

    emit taskDeleted(this->Taskid);
    this->close();
}

void TaskInfo::onUpdateData() {
    QSqlQuery query;

    query.prepare("UPDATE tasks SET title = :title,  description = :description, due_date = :due_date, priority = :priority, category_name = :category_name, category_color = :category_color, category_icon = :category_icon WHERE id = :id");

    query.bindValue(":title", this->title->text());
    query.bindValue(":description", this->description->text());

    qDebug() << "Due date (for DB):" << dateTime.toString("yyyy-MM-dd HH:mm:ss");

    query.bindValue(":due_date", this->dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":priority", this->priority->text());
    query.bindValue(":category_name", this->category->text());

    query.bindValue(":category_color", this->categoryColor);

    QByteArray iconBytes;
    QPixmap pixmap = this->category->icon().pixmap(64, 64);
    QBuffer buffer(&iconBytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    query.bindValue(":category_icon", iconBytes);
    query.bindValue(":id", this->Taskid);

    if (!query.exec()) {
        qDebug() << "Failed to update data: " << query.lastError();
        return;
    }

    emit onChangeUI();
    this->close();
}

void TaskInfo::loadTaskData(const Task& task) {

    qDebug() << "CAT_NAME:" << task.categoryName;

    this->title->setText(task.title);
    this->description->setText(task.description);
    this->dateTime = task.dueDate;
    this->taskTime->setText(task.dueDate.toString("dd.MM.yyyy 'at' hh:mm"));

    this->priority->setText(QString::number(task.priority));

    this->category->setText(task.categoryName);

    qDebug() << "CATEGORY:" << this->category->text();

    this->category->setIcon(task.categoryIcon);
    this->categoryColor = task.categoryColor;
    this->category->setStyleSheet("font-size: 13px; color: #fff; text-align: center; border-radius: 5px; height: 40px; min-width: 118px; max-width: 118px; "
                                  "background-color: " + categoryColor.name());
}

