#include "priorityitemwidget.h"

PriorityItemWidget::PriorityItemWidget(const int priori, QWidget* parent)
    : QWidget(parent), priorityInt(priori) {

    this->setStyleSheet("width: 64px; height: 64px; background-color: #272727;");

    this->iconBtn = new QToolButton(this);
    this->iconBtn->setIcon(QIcon(":/icons/red-flag.png"));
    this->iconBtn->setIconSize(QSize(24,24));
    this->iconBtn->setText(QString::number(priorityInt));
    this->iconBtn->setStyleSheet("font-size: 13px; color: #fff;");
    this->iconBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // this->priority = new QLabel(QString::number(priorityInt), this);
    // this->priority->setStyleSheet("font-size: 14px; color: #fff");

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(this->iconBtn, 0, Qt::AlignHCenter);
    // this->mainLayout->addWidget(this->priority, 0, Qt::AlignHCenter);
    this->mainLayout->setContentsMargins(0,0,0,0);
    this->mainLayout->setAlignment(Qt::AlignCenter);

    connect(this->iconBtn, &QToolButton::clicked, this, [this] {
        emit clickedWithId(this->priorityInt);
        emit openPriorityWnd();
    });
}

int PriorityItemWidget::getPriority() const {
    return this->priorityInt;
}
