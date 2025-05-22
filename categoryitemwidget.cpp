#include "categoryitemwidget.h"

CategoryItemWidget::CategoryItemWidget(const QString& nameStr, const QColor& colorC, const QIcon& iconC, QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(4);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);

    this->icon = iconC;
    this->color = colorC;

    QLabel* icoLabel = new QLabel(this);
    icoLabel->setFixedSize(64,64);
    icoLabel->setPixmap(icon.pixmap(48,48));
    icoLabel->setAlignment(Qt::AlignCenter);
    icoLabel->setStyleSheet("border-radius: 6px; background-color: " + color.name());

    this->name = new QLabel(nameStr, this);
    this->name->hide();

    QLabel* nameLabel = new QLabel(nameStr, this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-size: 12px");

    layout->addWidget(icoLabel);
    layout->addWidget(nameLabel);
}

QString CategoryItemWidget::getName() const {
    return this->name->text();
}

QColor CategoryItemWidget::getColor() const {
    return this->color;
}

QIcon CategoryItemWidget::getIcon() const {
    return this->icon;
}

void CategoryItemWidget::mousePressEvent(QMouseEvent *event) {
    emit itemClicked();

    QWidget::mousePressEvent(event);
}
