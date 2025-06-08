#include "helpandfeedbackwnd.h"

HelpAndFeedbackWnd::HelpAndFeedbackWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: #121212;");
    this->setFixedSize(375, 300);
    this->wndTitle = new QLabel("Help & Feedback", this);
    this->wndTitle->setStyleSheet("font-size: 17px; color: #fff");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->git = new QLabel(this);
    this->git->setText(R"(
        <a href="https://github.com/Jeyleyka" style="text-decoration: none;">
            <img src=":/icons/github.png" height="32" width="32" style="vertical-align:middle;">
            <span style="margin-left: 5px; color: #fff; font-size: 17px;">GitHub</span>
        </a>
    )");
    this->git->setTextFormat(Qt::RichText);
    this->git->setTextInteractionFlags(Qt::TextBrowserInteraction);
    this->git->setOpenExternalLinks(true);

    this->inst = new QLabel(this);
    this->inst->setText(R"(
        <a href="https://www.instagram.com/jeyleyka" style="text-decoration: none;">
            <img src=":/icons/instagram.png" height="32" width="32" style="vertical-align:middle;">
            <span style="margin-left: 5px; color: #fff; font-size: 17px;">Instagram</span>
        </a>
    )");
    this->inst->setTextFormat(Qt::RichText);      // Включаем HTML
    this->inst->setTextInteractionFlags(Qt::TextBrowserInteraction); // Разрешаем клик
    this->inst->setOpenExternalLinks(true);

    this->tg = new QLabel(this);
    this->tg->setText(R"(
        <a href="https://t.me/JlebCDev" style="text-decoration: none;">
            <img src=":/icons/telegram.png" height="32" width="32" style="vertical-align:middle;">
            <span style="margin-left: 5px; color: #fff; font-size: 17px;">Telegram</span>
        </a>
    )");
    this->tg->setStyleSheet("font-size: 16px;");
    this->tg->setTextFormat(Qt::RichText);      // Включаем HTML
    this->tg->setTextInteractionFlags(Qt::TextBrowserInteraction); // Разрешаем клик
    this->tg->setOpenExternalLinks(true);

    this->inform = new QLabel(tr("Write to me or subscribe to one of the social networks, I will be very pleased"), this);
    this->inform->setStyleSheet("font-size: 16px;");
    this->inform->setAlignment(Qt::AlignHCenter);
    this->inform->setWordWrap(true);

    ThemeManager::instance().loadTheme();

    this->closeBtn = new QPushButton(tr("close"), this);
    this->closeBtn->setStyleSheet("width: 330px; height: 40px; background-color: #8182DE; border-radius: 5px;");

    QColor color = ThemeManager::instance().buttonColor();
    this->closeBtn->setStyleSheet("width: 330px; height: 40px; border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->closeBtn->setStyleSheet("width: 330px; height: 40px; border-radius: 5px; background-color: " + color.name());
    });

    connect(this->closeBtn, &QPushButton::clicked, this, &HelpAndFeedbackWnd::close);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(15,15,15,15);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);
    this->mainLayout->setSpacing(10);
    this->mainLayout->addWidget(this->git);
    this->mainLayout->setSpacing(10);
    this->mainLayout->addWidget(this->inst);
    this->mainLayout->setSpacing(10);
    this->mainLayout->addWidget(this->tg);
    this->mainLayout->setSpacing(10);
    this->mainLayout->addWidget(this->inform);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->closeBtn);

    this->setLayout(this->mainLayout);
}

