#include "navigationbar.h"

NavigationBar::NavigationBar(QWidget* parent)
    : QWidget(parent)
{
    this->container = new QWidget(this);

    this->container->setStyleSheet("background-color: #363636;");
    this->container->setMinimumWidth(0);
    this->container->setFixedHeight(90);
    this->container->setMaximumWidth(QWIDGETSIZE_MAX);
    this->container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->indexBtn = new QPushButton(this);
    this->indexBtn->setIcon(QIcon(":/icons/home.png"));
    this->indexBtn->setIconSize(QSize(26,26));
    this->indexBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    connect(this->indexBtn, &QPushButton::clicked, this, [this] {
        emit this->switchToIndex();
    });

    this->indexLabel = new QLabel("Index", this);
    this->indexLabel->setStyleSheet("font-size: 14px;");

    auto* indexLayout = new QVBoxLayout();
    indexLayout->addWidget(this->indexBtn);
    indexLayout->addWidget(this->indexLabel);
    indexLayout->setAlignment(Qt::AlignCenter);
    indexLayout->setContentsMargins(0,15,0,0);

    this->calendarBtn = new QPushButton(this);
    this->calendarBtn->setIcon(QIcon(":/icons/calendar.png"));
    this->calendarBtn->setIconSize(QSize(26,26));
    this->calendarBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    connect(this->calendarBtn, &QPushButton::clicked, this, [this] {
        emit this->switchToCalendar();
    });

    this->calendarLabel = new QLabel("Calendar", this);
    this->calendarLabel->setStyleSheet("font-size: 14px;");

    auto* calendarLayout = new QVBoxLayout();
    calendarLayout->addWidget(this->calendarBtn);
    calendarLayout->addWidget(this->calendarLabel);
    calendarLayout->setAlignment(Qt::AlignCenter);
    calendarLayout->setContentsMargins(20, 15, 30, 0);

    this->addTaskButton = new QPushButton(this);
    this->addTaskButton->setIcon(QIcon(":/icons/plus.png"));
    this->addTaskButton->setIconSize(QSize(30,30));
    this->addTaskButton->setStyleSheet("width: 64px; height: 64px; background-color: #8687E7; border-radius: 32px;");

    connect(this->addTaskButton, &QPushButton::clicked, this, [this] {
        emit this->onShowTaskDialog();
    });

    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(this->addTaskButton);

    this->focusBtn = new QPushButton(this);
    this->focusBtn->setIcon(QIcon(":/icons/clock.png"));
    this->focusBtn->setIconSize(QSize(35,35));
    this->focusBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    connect(this->focusBtn, &QPushButton::clicked, this, [this] {
        emit this->switchToFocus();
    });

    this->focusLabel = new QLabel("Focuse", this);
    this->focusLabel->setStyleSheet("font-size: 14px;");

    auto* focusLayout = new QVBoxLayout();
    focusLayout->addWidget(this->focusBtn);
    focusLayout->addWidget(this->focusLabel);
    focusLayout->setAlignment(Qt::AlignCenter);
    focusLayout->setContentsMargins(30,15,20,0);

    this->profileBtn = new QPushButton(this);
    this->profileBtn->setIcon(QIcon(":/icons/human.png"));
    this->profileBtn->setIconSize(QSize(32,32));
    this->profileBtn->setStyleSheet("width: 30px; height: 50px; border: none; background-color: transparent");

    connect(this->profileBtn, &QPushButton::clicked, this, [this] {
        emit this->switchToProfile();
    });

    this->profileLabel = new QLabel("Profile", this);
    this->profileLabel->setStyleSheet("font-size: 14px;");

    auto* profileLayout = new QVBoxLayout();
    profileLayout->addWidget(this->profileBtn);
    profileLayout->addWidget(this->profileLabel);
    profileLayout->setAlignment(Qt::AlignCenter);
    profileLayout->setContentsMargins(0, 15, 0, 0);

    auto* mainLayout = new QHBoxLayout(this->container);
    mainLayout->addLayout(indexLayout);
    mainLayout->addLayout(calendarLayout);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(focusLayout);
    mainLayout->addLayout(profileLayout);
    mainLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(container);
    outerLayout->setContentsMargins(0, 0, 0, 0);
}
