#include "infofornewuserwnd.h"

InfoForNewUserWnd::InfoForNewUserWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(375, 450);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background-color: #111111; border-radius: 5px;");

    QWidget* container = new QWidget(this);

    this->skipSlides = new QPushButton("SKIP", container);
    this->skipSlides->setFixedSize(90, 48);
    this->skipSlides->setStyleSheet("background-color: transparent; border: none; color: #797979; font-size: 16px;");

    this->stackedWidget = new QStackedWidget(container);

    this->createSlide(this->manageSlide, "manage-icon.svg", QSize(213, 277), "Manage your tasks", "You can easily manage all of your daily tasks in DoMe", 0);
    this->createSlide(this->dailySlide, "daily-icon.svg", QSize(271, 296), "Create daily routine", "In TaskFlow you can create your personalized routine to stay productive", 1);
    this->createSlide(this->orgonaizeSlide, "org-icon.svg", QSize(257, 251), "Organize your tasks", "You can organize your daily tasks by adding your tasks into separate categories", 2);

    this->stackedWidget->addWidget(this->manageSlide);
    this->stackedWidget->addWidget(this->dailySlide);
    this->stackedWidget->addWidget(this->orgonaizeSlide);
    this->stackedWidget->setCurrentIndex(0);

    connect(this->skipSlides, &QPushButton::clicked, this, [this] {
        this->stackedWidget->setCurrentIndex(2);
    });

    this->backSlide = new QPushButton("BACK", container);
    this->backSlide->setFixedSize(90, 48);
    this->backSlide->setStyleSheet("background-color: transparent; border: none; color: #797979; font-size: 16px;");

    connect(this->backSlide, &QPushButton::clicked, this, [this] {
        if (this->stackedWidget->currentIndex() != 0)
            this->stackedWidget->setCurrentIndex(this->stackedWidget->currentIndex() - 1);
    });

    this->nextSlide = new QPushButton("NEXT", container);
    this->nextSlide->setFixedSize(90, 48);
    this->nextSlide->setStyleSheet("background-color: #8875FF; color: #fff; font-size: 16px;");

    connect(this->nextSlide, &QPushButton::clicked, this, [this] {
        this->stackedWidget->setCurrentIndex(this->stackedWidget->currentIndex() + 1);

        if (this->stackedWidget->currentIndex() == 2)
        {
            this->nextSlide->setText("GET STARTED");
            this->nextSlide->setFixedSize(150, 48);

            connect(this->nextSlide, &QPushButton::clicked, this, [this] {
                Intro* intro = new Intro();
                intro->show();
                this->close();
            });
        }
    });

    QHBoxLayout* backAndNextLayout = new QHBoxLayout();
    backAndNextLayout->addWidget(this->backSlide);
    backAndNextLayout->addStretch();
    backAndNextLayout->addWidget(this->nextSlide);

    this->mainLayout = new QVBoxLayout(container);
    this->mainLayout->addWidget(this->skipSlides, 0, Qt::AlignLeft);
    this->mainLayout->addWidget(this->stackedWidget);
    this->mainLayout->addLayout(backAndNextLayout);
    this->mainLayout->setContentsMargins(10, 10, 10, 10);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(container);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
}

void InfoForNewUserWnd::createSlide(QWidget*& slide, QString iconName, QSize size, QString titleText, QString descText, int index) {
    slide = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(slide);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(15);

    QPushButton* iconBtn = new QPushButton(slide);
    iconBtn->setIcon(QIcon(":/icons/" + iconName));
    iconBtn->setIconSize(size);
    iconBtn->setFixedSize(size);
    iconBtn->setStyleSheet("background-color: transparent; border: none");

    QLabel* title = new QLabel(titleText, slide);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #fff;");
    title->setAlignment(Qt::AlignHCenter);

    QLabel* description = new QLabel(descText, slide);
    description->setStyleSheet("font-size: 19px; color: #fff;");
    description->setAlignment(Qt::AlignHCenter);
    description->setWordWrap(true);

    // 🔹 Создаём отдельные кнопки для этого слайда
    QHBoxLayout* localBtnsLayout = new QHBoxLayout();
    localBtnsLayout->setAlignment(Qt::AlignHCenter);
    localBtnsLayout->setSpacing(8);

    for (int i = 0; i < 3; ++i) {
        QPushButton* btn = new QPushButton();
        btn->setFixedSize(26, 4);
        btn->setCheckable(true);
        btn->setStyleSheet(
            "QPushButton { border-radius: 2px; background-color: #919191; }"
            "QPushButton:checked { background-color: #C0C0C0; }"
            );

        if (i == index) {
            btn->setChecked(true);
            activeBtn = btn;  // первая активная кнопка
        }

        connect(btn, &QPushButton::clicked, this, [this, btn, i]() {
            this->stackedWidget->setCurrentIndex(i);

            if (this->activeBtn && this->activeBtn != btn) {
                this->activeBtn->setChecked(false);
            }

            btn->setChecked(true);
            this->activeBtn = btn;
        });

        localBtnsLayout->addWidget(btn);
    }

    layout->addWidget(iconBtn, 0, Qt::AlignHCenter);
    layout->addLayout(localBtnsLayout);  // 🔹 Вставляем кнопки внутрь текущего слайда
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addStretch(1);
}
