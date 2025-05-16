#include "taskui.h"

TaskUI::TaskUI(QString titleStr, QString desc, QString createData, QString priorityStr, QWidget* parent)
    : QWidget(parent)
{
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QFrame* frame = new QFrame(this);

    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setStyleSheet("QFrame {"
                               "border-radius: 5px;"
                               "background-color: #363636;"
                               "padding: 10px;"
                                // "min-width: 50px;"
                               "}");

    auto* mainLayout = new QHBoxLayout(frame);

    // Иконка
    QLabel* priorityLabel = new QLabel(frame);
    QPixmap icon(priorityStr);
    icon = icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    priorityLabel->setStyleSheet("border: none");
    priorityLabel->setFixedSize(50, 50);
    priorityLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    priorityLabel->setPixmap(icon);

    QVBoxLayout* textLayout = new QVBoxLayout(frame);


    // Текст
    this->title = new QLabel(titleStr, frame);
    this->title->setStyleSheet("font-size: 17px; color: white; border: none;");
    this->title->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->creationDate = new QLabel(createData, frame);
    this->creationDate->setStyleSheet("color: #919191; border: none;");

    textLayout->addWidget(this->title);
    textLayout->addWidget(this->creationDate);

    this->successTask = new QPushButton("success", frame);
    this->successTask->setStyleSheet("background-color: #0d8b04; width: 100px; height: 25px; color: #fff;");

    this->showDescription = new QPushButton(frame);
    this->showDescription->setIcon(QIcon(":/icons/red-flag.png"));
    this->showDescription->setIconSize(QSize(20,20));
    this->showDescription->setStyleSheet("background-color: transparent; width: 25px; height: 25px;");

    // Добавляем в layout
    mainLayout->addWidget(priorityLabel);
    mainLayout->addLayout(textLayout);
    mainLayout->addWidget(this->successTask, 2, Qt::AlignBottom);
    mainLayout->addWidget(this->showDescription, 3, Qt::AlignBottom);
    mainLayout->setSpacing(0);
    mainLayout->addStretch(0);
    mainLayout->setContentsMargins(0,0,0,0);

    // Фон на весь TaskUI
    // this->setStyleSheet("background-color: #2D2D2D; border-radius: 8px;");
}

void TaskUI::mousePressEvent(QMouseEvent *event) {
    emit this->taskClicked();
    QWidget::mousePressEvent(event);
}
