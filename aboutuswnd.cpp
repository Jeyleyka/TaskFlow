#include "aboutuswnd.h"

AboutUsWnd::AboutUsWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: #121212;");
    this->setFixedSize(375, 500);
    this->wndTitle = new QLabel("About Developer", this);
    this->wndTitle->setStyleSheet("font-size: 17px; color: #fff");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    // Это приложение — результат моего стремления объединить минимализм, функциональность и удобство.
    // Если у тебя есть идеи, замечания или ты просто хочешь сказать «привет» — не стесняйся связаться со мной!

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->intro = new QLabel("Hi, i'm Gleb, i'm 17 as of 2025. I've been studying C++ for 3 years, i developed this app to make everyday tasks easier and more "
                             "enjoyable.", this);
    this->intro->setStyleSheet("font-size: 16px;");
    this->intro->setWordWrap(true);
    this->intro->setAlignment(Qt::AlignHCenter);

    this->middle = new QLabel("I passionate about creating convenient and beautiful tools that really help in life. This app is the result of my desire to combine minimalism, functionality and convenience.", this);
    this->middle->setStyleSheet("font-size: 16px;");
    this->middle->setWordWrap(true);
    this->middle->setAlignment(Qt::AlignHCenter);

    this->gratitude = new QLabel("If you have an idea or just want to say 'hi' - feel free to contact me. I would "
                                 "be glad to hear any feedback on my app or know that someone is using it", this);
    this->gratitude->setStyleSheet("font-size: 16px;");
    this->gratitude->setWordWrap(true);
    this->gratitude->setAlignment(Qt::AlignHCenter);

    ThemeManager::instance().loadTheme();

    this->closeBtn = new QPushButton("close", this);
    this->closeBtn->setStyleSheet("width: 330px; height: 40px; background-color: #8182DE; border-radius: 5px;");

    QColor color = ThemeManager::instance().buttonColor();
    this->closeBtn->setStyleSheet("width: 330px; height: 40px; border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->closeBtn->setStyleSheet("width: 330px; height: 40px; border-radius: 5px; background-color: " + color.name());
    });

    connect(this->closeBtn, &QPushButton::clicked, this, &AboutUsWnd::close);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(15,15,15,15);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);
    this->mainLayout->addWidget(this->intro);
    this->mainLayout->setSpacing(8);
    this->mainLayout->addWidget(this->middle);
    this->mainLayout->setSpacing(8);
    this->mainLayout->addWidget(this->gratitude);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->closeBtn);

    this->setLayout(this->mainLayout);
}
