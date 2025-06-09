#include "profilesettingswidget.h"

ProfileSettingsWidget::ProfileSettingsWidget(const QIcon& icon, const QString& str, QWidget* parent)
    : QWidget(parent)
{
    this->setFixedSize(370, 48);
    this->setAutoFillBackground(true);

    this->container = new QWidget(this);
    this->container->setStyleSheet("background-color: #363636; border-radius: 5px;");
    this->container->setFixedSize(370, 48);

    ThemeManager::instance().loadTheme();

    QColor color = ThemeManager::instance().widgetsColor();
    this->container->setStyleSheet("border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().widgetsColor();
        this->container->setStyleSheet("border-radius: 5px; background-color: " + color.name());
    });

    connect(&ThemeManager::instance(), &ThemeManager::widgetsChanged, this, [this](const QColor& color) {
        QString style = QString("background-color: %1; border-radius: 5px;").arg(color.name());
        this->container->setStyleSheet(style);
    });

    this->iconBtn = new QPushButton(this);
    this->iconBtn->setIcon(icon);
    this->iconBtn->setIconSize(QSize(24,24));
    this->iconBtn->setStyleSheet("background-color: transparent; border: none;");
    this->iconBtn->setFixedSize(24,24);

    this->text = new QLabel(str, this);
    this->text->setStyleSheet("font-size: 16px; color: #fff");
    this->text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->arrow = new QPushButton(">", this);
    this->arrow->setStyleSheet("background-color: transparent; border: none; font-size: 24px;");
    this->arrow->setFixedSize(24,24);

    this->mainLayout = new QHBoxLayout(this->container);
    mainLayout->setContentsMargins(12, 0, 12, 0); // отступы слева/справа
    mainLayout->setSpacing(10);
    this->mainLayout->addWidget(this->iconBtn);
    this->mainLayout->addWidget(this->text);
    this->mainLayout->addWidget(this->arrow);

    QVBoxLayout* wrapperLayout = new QVBoxLayout(this);
    wrapperLayout->addWidget(container);
    wrapperLayout->setContentsMargins(0, 0, 0, 0);
}

void ProfileSettingsWidget::mousePressEvent(QMouseEvent *event) {
    emit this->onClicked();

    QWidget::mousePressEvent(event);
}
