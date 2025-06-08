#include "intro.h"

Intro::Intro(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    this->container = new QWidget(this);

    ThemeManager::instance().loadTheme();

    QColor color = ThemeManager::instance().backgroundColor();
    QPalette pal = container->palette();
    pal.setColor(QPalette::Window, color);
    container->setAutoFillBackground(true);
    container->setPalette(pal);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().backgroundColor();
        QPalette pal = container->palette();
        pal.setColor(QPalette::Window, color);
        this->container->setAutoFillBackground(true);
        this->container->setPalette(pal);
    });

    this->prevBtn = new QPushButton("<", this);
    this->prevBtn->setStyleSheet("background-color: transparent; border: none; font-size: 30px;");

    this->closeBtn = new QPushButton("×", this);
    this->closeBtn->setStyleSheet("background-color: transparent; border: none; font-size: 31px;");

    connect(this->closeBtn, &QPushButton::clicked, this, &Intro::close);

    this->btnsLayout = new QHBoxLayout();
    this->btnsLayout->addWidget(this->prevBtn);
    this->btnsLayout->addStretch(1);
    this->btnsLayout->addWidget(this->closeBtn);

    this->wndTitle = new QLabel(tr("Welcome to TaskFlow"), this);
    this->wndTitle->setStyleSheet("font-size: 23px; font-weight: 600; color: #fff; margin-top: 15px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->subtitle = new QLabel(tr("Please login to your account or create\n new account to continue"), this);
    this->subtitle->setWordWrap(true);  // включи перенос строк
    this->subtitle->setStyleSheet("font-size: 16px; color: #898989");
    this->subtitle->setAlignment(Qt::AlignHCenter);

    this->loginBtn = new QPushButton(tr("LOGIN"), this);
    this->loginBtn->setStyleSheet("width: 350px; height: 40px; background-color: #8687E7; border-radius: 5px; margin-top: 180px;");

    QColor btnColor = ThemeManager::instance().buttonColor();
    this->loginBtn->setStyleSheet("width: 350px; height: 40px; border-radius: 5px; margin-top: 180px; background-color: " + btnColor.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor btnColor = ThemeManager::instance().buttonColor();
        this->loginBtn->setStyleSheet("width: 350px; height: 40px; border-radius: 5px; margin-top: 180px; background-color: " + btnColor.name());
    });

    connect(this->loginBtn, &QPushButton::clicked, this, [this] {
        Login* loginWnd = new Login();
        loginWnd->show();
        this->close();
    });

    this->createAccBtn = new QPushButton(tr("CREATE ACCOUNT"), this);
    this->createAccBtn->setStyleSheet("width: 350px; height: 40px; background-color: transparent; border: 1px solid #8687E7; border-radius: 5px; margin-top: 10px;");

    QColor inBtnColor = ThemeManager::instance().buttonColor();
    this->createAccBtn->setStyleSheet("width: 350px; height: 40px; background-color: transparent; border-radius: 5px; margin-top: 10px; "
                                      "border: 1px solid " + inBtnColor.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor inBtnColor = ThemeManager::instance().buttonColor();
        this->createAccBtn->setStyleSheet("width: 350px; height: 40px; background-color: transparent; border-radius: 5px; margin-top: 10px; "
                                          "border: 1px solid " + inBtnColor.name());
    });

    connect(this->createAccBtn, &QPushButton::clicked, this, [this] {
        CreateAccount* createAccWnd = new CreateAccount();
        createAccWnd->show();
        this->close();
    });


    this->mainLayout = new QVBoxLayout(this->container);
    this->mainLayout->addLayout(this->btnsLayout);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->subtitle);
    this->mainLayout->addStretch(1);
    this->mainLayout->addWidget(this->loginBtn);
    this->mainLayout->addWidget(this->createAccBtn);

    this->setFixedSize(375, 450);
}

Intro::~Intro() {}
