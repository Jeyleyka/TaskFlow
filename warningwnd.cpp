#include "warningwnd.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QGuiApplication>

WarningWnd::WarningWnd(const QString& text, QWidget* parent)
    : QDialog(parent), warningText(new QLabel(text)), animation(nullptr)
{
    // Убираем рамку и делаем окно модальным
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setModal(true);

    // Получаем размеры экрана
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int windowHeight = 200;

    // Устанавливаем размеры окна на ширину экрана
    this->setFixedSize(screenWidth, windowHeight);

    // Ставим окно по центру экрана по вертикали
    int centerY = screenGeometry.center().y() - (windowHeight / 2);
    this->move(0, centerY);

    // Загружаем тему
    ThemeManager::instance().loadTheme();

    // Основной layout окна
    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(0, 0, 0, 0);

    // Контейнер с фоном
    this->container = new QWidget(this);
    this->container->setFixedSize(screenWidth, windowHeight);

    this->container->setStyleSheet("background-color: #121212");

    // Слушаем изменение темы
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().backgroundColor();
        this->container->setStyleSheet("background-color: " + color.name());
    });

    // Layout внутри контейнера
    this->containerLayout = new QVBoxLayout(this->container);
    this->containerLayout->setContentsMargins(0, 30, 0, 30);
    this->containerLayout->setSpacing(20);

    // Текст предупреждения
    this->warningText->setStyleSheet("font-size: 17px; color: white;");
    this->warningText->setAlignment(Qt::AlignCenter);
    this->containerLayout->addWidget(this->warningText);

    // Кнопка
    this->closeBtn = new QPushButton(tr("Close"), this);
    this->closeBtn->setFixedSize(150, 35);
    this->closeBtn->setStyleSheet("border-radius: 5px; background-color: " + ThemeManager::instance().buttonColor().name());

    connect(this->closeBtn, &QPushButton::clicked, this, [this] {
        QPropertyAnimation* fadeOut = new QPropertyAnimation(this, "windowOpacity");
        fadeOut->setDuration(300);  // длительность анимации
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);
        fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

        connect(fadeOut, &QPropertyAnimation::finished, this, &WarningWnd::close);
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        this->closeBtn->setStyleSheet("border-radius: 5px; background-color: " + ThemeManager::instance().buttonColor().name());
    });

    this->containerLayout->addWidget(this->closeBtn, 0, Qt::AlignHCenter);

    this->mainLayout->addWidget(this->container);
}

void WarningWnd::showWithAnimation() {
    // Центрируем окно на экране
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    // Устанавливаем прозрачность на 0 (полностью невидимо)
    this->setWindowOpacity(0.0);
    this->show();

    // Анимация появления (fade-in)
    QPropertyAnimation* fadeAnim = new QPropertyAnimation(this, "windowOpacity");
    fadeAnim->setDuration(400);  // длительность анимации
    fadeAnim->setStartValue(0.0);
    fadeAnim->setEndValue(1.0);
    fadeAnim->setEasingCurve(QEasingCurve::InOutQuad);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
