#include "deletetaskwnd.h"

DeleteTaskWnd::DeleteTaskWnd(QString taskTitle, int taskId, QWidget* parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(327, 198);
    this->setStyleSheet("background-color: #121212; border-radius: 5px;");

    this->container = new QWidget(this);

    this->containerLayout = new QVBoxLayout(this->container);
    this->containerLayout->setContentsMargins(10,10,10,10);

    this->wndTitle = new QLabel("Delete Task");
    this->wndTitle->setStyleSheet("font-size: 16px; color: #fff");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame();
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->warning = new QLabel("Are you sure you want to delete this task?");
    this->warning->setStyleSheet("font-size: 15px; color: #fff");
    this->warning->setAlignment(Qt::AlignHCenter);

    this->taskTitle = new QLabel("Task title: " + taskTitle);
    this->taskTitle->setStyleSheet("font-size: 15px; color: #fff");
    this->taskTitle->setAlignment(Qt::AlignHCenter);

    this->btnsLayout = new QHBoxLayout();

    this->cancelBtn = new QPushButton("Cancel");
    this->cancelBtn->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: #8182DC");

    connect(this->cancelBtn, &QPushButton::clicked, this, &DeleteTaskWnd::close);

    this->deleteBtn = new QPushButton("Delete");
    this->deleteBtn->setStyleSheet("width: 153px; height: 48px; background-color: #8687E7");

    connect(this->deleteBtn, &QPushButton::clicked, this, [this, taskId] {
        emit deleteTask();
        this->close();
    });

    this->btnsLayout->addWidget(this->cancelBtn);
    this->btnsLayout->addWidget(this->deleteBtn);

    this->containerLayout->addWidget(this->wndTitle);
    this->containerLayout->addWidget(this->line);
    this->containerLayout->addWidget(this->warning);
    this->containerLayout->addWidget(this->taskTitle);
    this->containerLayout->addLayout(this->btnsLayout);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(this->container);

    this->setLayout(this->mainLayout);
}
