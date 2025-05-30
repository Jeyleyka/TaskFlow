#include "appsettings.h"

AppSettings::AppSettings(QWidget* parent)
    : QDialog(parent)
{
    this->setFixedSize(375, 500);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: #121212;"); // фон для всего окна

    // Назад кнопка
    this->prevBtn = new QPushButton("<", this);
    this->prevBtn->setStyleSheet("font-size: 24px; background-color: transparent; border: none; color: white;");
    this->prevBtn->setFixedSize(24, 24);
    connect(this->prevBtn, &QPushButton::clicked, this, &AppSettings::close);

    // Заголовок
    this->wndTitle = new QLabel("Settings", this);
    this->wndTitle->setStyleSheet("font-size: 20px; color: #fff;");
    this->wndTitle->setAlignment(Qt::AlignCenter);

    // Создаём header layout
    this->headerLayout = new QHBoxLayout();
    this->headerLayout->setContentsMargins(10, 10, 10, 10);
    this->headerLayout->setSpacing(0);

    QSpacerItem* leftSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* rightSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    this->headerLayout->addWidget(this->prevBtn);
    this->headerLayout->addItem(leftSpacer);
    this->headerLayout->addWidget(this->wndTitle);
    this->headerLayout->addItem(rightSpacer);

    // Центрируем wndTitle в layout за счёт равных спейсеров
    this->headerLayout->setStretch(1, 1); // левый spacer
    this->headerLayout->setStretch(3, 1); // правый spacer
    this->headerLayout->setAlignment(Qt::AlignTop);

    // Виджеты настроек
    this->changeAppColor = new ProfileSettingsWidget(QIcon(":/icons/brush.png"), "Change app color", this);
    this->changeAppTypography = new ProfileSettingsWidget(QIcon(":/icons/t.png"), "Change app typography", this);
    this->changeAppLanguage = new ProfileSettingsWidget(QIcon(":/icons/language.png"), "Change app language", this);

    // Основной layout
    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(16, 16, 16, 16); // отступы по краям
    this->mainLayout->setSpacing(16);

    this->mainLayout->addLayout(this->headerLayout);
    this->mainLayout->addWidget(this->changeAppColor, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->changeAppTypography, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->changeAppLanguage, 0, Qt::AlignHCenter);

    this->setLayout(this->mainLayout);

}

AppSettings::~AppSettings() {}
