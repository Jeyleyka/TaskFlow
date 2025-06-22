#include "logownd.h"

LogoWnd::LogoWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(300, 350);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background-color: #111111; border-radius: 5px;");

    this->db = new DatabaseManager;
    this->db->initializeDatabase();

    QWidget* container = new QWidget(this);

    this->logoBtn = new QPushButton(this);
    this->logoBtn->setIcon(QIcon(":/icons/logo-transparent.png"));
    this->logoBtn->setIconSize(QSize(180, 230));
    this->logoBtn->setStyleSheet("background-color: transparent; border: none");

    this->mainLayout = new QVBoxLayout(container);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->logoBtn);
    this->mainLayout->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(container);

    this->setLayout(layout);

    QTimer::singleShot(0, this, [this]() {
        this->showWithAnimation();
    });

    QTimer::singleShot(3200, this, [this] {
        if (this->db->isUsersTableExists())
        {
            Intro* intro = new Intro();
            intro->show();
        }
        else
        {
            InfoForNewUserWnd* info = new InfoForNewUserWnd();
            info->show();
        }
        this->close();
    });
}

void LogoWnd::showWithAnimation() {
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this->logoBtn);
    this->logoBtn->setGraphicsEffect(effect);

    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(900);
    anim->setStartValue(0.0);
    anim->setEndValue(1.5);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
