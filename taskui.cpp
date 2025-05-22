#include "taskui.h"

TaskUI::TaskUI(QString titleStr, QString desc, QString createData, int priority,
               QString categoryName, QColor categoryColor, QIcon categoryIcon,
               QWidget* parent)
    : QWidget(parent), counter(0), categoryIcon(categoryIcon), priority(priority), categoryColor(categoryColor)
{
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setStyleSheet("QFrame {"
                         "border-radius: 5px;"
                         "background-color: #363636;"
                         "padding: 10px;"
                         "min-width: 180px;"
                         "height: 72px;"
                         "}");

    auto* mainLayout = new QHBoxLayout(frame);
    QVBoxLayout* textLayout = new QVBoxLayout();

    this->circle = new QPushButton(frame);
    this->circle->setIcon(QIcon(":/icons/empty-circle.png"));
    this->circle->setIconSize(QSize(24,24));
    this->circle->setStyleSheet("border: none;");

    connect(circle, &QPushButton::clicked, this, [this]{
        if (this->counter == 0) {
            this->circle->setIcon(QIcon(":/icons/blue-circle.png"));
            this->circle->setIconSize(QSize(24,24));
            this->counter++;
        } else {
            this->circle->setIcon(QIcon(":/icons/empty-circle.png"));
            this->circle->setIconSize(QSize(24,24));
            this->counter--;
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

    // Виджет категории (иконка + текст)

    // QFrame* categoryFrame = new QFrame(frame);
    // categoryFrame->setFrameShape(QFrame::StyledPanel);
    // categoryFrame->setFrameShadow(QFrame::Raised);
    // categoryFrame->setStyleSheet("border-radius: 5px; padding: 10px; background-color: " + categoryColor.name());

    // QHBoxLayout* categoryLayout = new QHBoxLayout(categoryFrame);
    // categoryLayout->setContentsMargins(8, 0, 8, 0);
    // categoryLayout->setSpacing(6);
    // categoryLayout->setAlignment(Qt::AlignVCenter);

    // QPushButton* iconBtn = new QPushButton(categoryFrame);
    // // QPixmap iconPixmap = categoryIcon.pixmap(14, 14);
    // iconBtn->setIcon(categoryIcon);
    // iconBtn->setIconSize(QSize(14,14));
    // iconBtn->setFixedSize(14, 14);
    // // iconBtn->setScaledContents(true);

    // QLabel* textLabel = new QLabel(categoryName, categoryFrame);
    // textLabel->setStyleSheet("color: white; font-size: 13px;");
    // textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    // categoryLayout->addWidget(iconBtn);
    // categoryLayout->addWidget(textLabel);
    // categoryLayout->addStretch();

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

QString TaskUI::getCategoryName() const {
    return this->categoryLabel->text();
}

QColor TaskUI::getCategoryColor() const {
    return this->categoryColor;
}

QIcon TaskUI::getCategoryIcon() const {
    return this->categoryIcon;
}

int TaskUI::getPriority() const {
    return this->priority;
}

void TaskUI::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Mouse clicked";
    emit this->taskClicked();
    QWidget::mousePressEvent(event);
}
