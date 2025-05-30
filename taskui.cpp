#include "taskui.h"

TaskUI::TaskUI(QString titleStr, QString desc, QString createData, int priority,
               QString categoryName, QColor categoryColor, QIcon categoryIcon, int id,
               int completed, QWidget* parent)
    : QWidget(parent), createDate(createData), counter(completed), categoryIcon(categoryIcon), priority(priority), categoryColor(categoryColor), completed(0), taskID(id)
{
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setStyleSheet("QFrame {"
                         "border-radius: 5px;"
                         "background-color: #363636;"
                         "padding: 10px;"
                         "min-width: 900px;"
                         "max-width: 900px;"
                         "height: 50px;"
                         "}");

    auto* mainLayout = new QHBoxLayout(frame);
    QVBoxLayout* textLayout = new QVBoxLayout();

    qDebug() << "COUNTER: " << this->counter;

    this->circle = new QPushButton(frame);
    this->circle->setIcon(QIcon(":/icons/empty-circle.png"));
    this->circle->setIconSize(QSize(24,24));
    this->circle->setStyleSheet("border: none;");

    if (this->counter == 1) {
        this->circle->setIcon(QIcon(":/icons/blue-circle.png"));
        this->circle->setIconSize(QSize(24,24));
        this->counter--;
        this->completed = 0;
    } else {
        this->circle->setIcon(QIcon(":/icons/empty-circle.png"));
        this->circle->setIconSize(QSize(24,24));
        this->counter++;
        this->completed = 1;
    }

    connect(circle, &QPushButton::clicked, this, [this]{
        if (this->counter == 1) {
            this->circle->setIcon(QIcon(":/icons/blue-circle.png"));
            this->circle->setIconSize(QSize(24,24));
            this->counter--;
            this->completed = 0;
            this->updateData();
            emit onUpdateTaskToComplete();
        } else {
            this->circle->setIcon(QIcon(":/icons/empty-circle.png"));
            this->circle->setIconSize(QSize(24,24));
            this->counter++;
            this->completed = 1;
            this->updateData();
            emit onUpdateTaskToNotComplete();
        }
    });

    this->categoryLabel = new QLabel(categoryName);
    this->categoryLabel->hide();

    // Название задачи
    this->title = new QLabel(titleStr, frame);
    this->title->setStyleSheet("font-size: 17px; color: white; border: none;");
    this->title->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Дата создания
    this->creationDate = new QLabel(createData, frame);
    this->creationDate->setStyleSheet("color: #919191; border: none;");
    this->creationDate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textLayout->addWidget(this->title);
    textLayout->addWidget(this->creationDate);
    textLayout->addStretch();
    textLayout->setSpacing(0);

    // Иконка
    this->categoryBtn = new QPushButton(frame);
    this->categoryBtn->setIcon(QIcon(categoryIcon));
    this->categoryBtn->setIconSize(QSize(14,14));
    this->categoryBtn->setLayoutDirection(Qt::LeftToRight);
    this->categoryBtn->setText(categoryName);
    this->categoryBtn->setStyleSheet("min-width: 87px; min-height: 29px; margin-bottom: 4px; border-radius: 5px; font-size: 14px; color: #fff; background-color: " + categoryColor.name());
    // Добавляем в layout

    // Кнопка для показа описания
    this->showDescription = new QPushButton(frame);
    this->showDescription->setIcon(QIcon(":/icons/red-flag.png"));
    this->showDescription->setIconSize(QSize(15, 15));
    this->showDescription->setText(QString::number(this->priority));
    this->showDescription->setStyleSheet("background-color: transparent; width: 42px; height: 29px; margin-bottom: 3px;");


    mainLayout->addWidget(circle, Qt::AlignVCenter | Qt::AlignLeft);
    mainLayout->addLayout(textLayout);
    mainLayout->addStretch();  // для выравнивания влево
    mainLayout->addWidget(categoryBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    mainLayout->addWidget(this->showDescription, 0, Qt::AlignBottom);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

TaskUI::TaskUI() {}

void TaskUI::setTitle(QString& newTitle) {
    this->title->setText(newTitle);
}

void TaskUI::setDesc(QString& newDesc) {
    if (!this->description) {
        this->description = new QLabel(this);
        this->description->setStyleSheet("color: white;");
    }
    this->description->setText(newDesc);
}

void TaskUI::setCategory(QString name, QColor color, QIcon icon, int width, int height) {
    this->categoryBtn->setIcon(icon);
    this->categoryBtn->setIconSize(QSize(width, height));
    this->categoryBtn->setText(name);
    this->categoryBtn->setStyleSheet("min-width: 87px; min-height: 29px; margin-bottom: 4px; border-radius: 5px; font-size: 14px; color: #fff; background-color: " + color.name());
}

void TaskUI::setPriority(QString priority) const {
    this->showDescription->setText(priority);
}

QString TaskUI::getTitle() const {
    return this->title->text();
}

QString TaskUI::getCategoryName() const {
    return this->categoryLabel->text();
}

QColor TaskUI::getCategoryColor() const {
    return this->categoryColor;
}

QIcon TaskUI::getCategoryIcon() const {
    return this->categoryIcon;
}

QString TaskUI::getDate() const {
    return this->createDate;
}

int TaskUI::getPriority() const {
    return this->priority;
}

bool TaskUI::getCompleted() const {
    return !this->counter;
}

void TaskUI::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Mouse clicked";
    emit this->taskClicked();
    QWidget::mousePressEvent(event);
}

void TaskUI::updateData() {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");

    query.bindValue(":completed", !this->counter);
    query.bindValue(":id", this->taskID);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении задачи:" << query.lastError().text();
    }

    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id = :user_id AND completed = 1");
    query.bindValue(":user_id", UserSession::getUserId());
    if (query.exec() && query.next()) {
        int completedCount = query.value(0).toInt();
        qDebug() << "Completed tasks:" << completedCount;
    }
}
