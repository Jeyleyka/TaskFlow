#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget* parent)
    : QWidget(parent), currentDate(QDate::currentDate()), selectedDate(QDate::currentDate())
{
    this->setStyleSheet("margin-top: 10px; background-color: #363636; max-height: 130px;");
    QWidget* container = new QWidget(this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // растягивать

    QVBoxLayout* calendarLayout = new QVBoxLayout(container);
    calendarLayout->setContentsMargins(0, 0, 0, 0);
    calendarLayout->setSpacing(0);

    // ------- Верхняя панель со стрелками и месяцем -------
    QHBoxLayout* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->setSpacing(0);

    QPushButton* prevBtn = new QPushButton("<", container);
    prevBtn->setFixedSize(30, 30);
    prevBtn->setStyleSheet("border: none; color: white; font-size: 25px;");

    QPushButton* nextBtn = new QPushButton(">", container);
    nextBtn->setFixedSize(30, 30);
    nextBtn->setStyleSheet("border: none; color: white; font-size: 25px;");

    this->monthLabel = new QLabel(container);
    this->monthLabel->setStyleSheet("color: white; font-size: 16px;");
    this->monthLabel->setAlignment(Qt::AlignCenter);

    this->yearLabel = new QLabel(container);
    this->yearLabel->setStyleSheet("color: #727272; font-size: 14px;");
    this->yearLabel->setAlignment(Qt::AlignCenter);

    auto* dateLayout = new QVBoxLayout;
    dateLayout->addWidget(this->monthLabel);
    dateLayout->setSpacing(0);
    dateLayout->addWidget(this->yearLabel);
    dateLayout->setContentsMargins(0,0,0,0);

    connect(prevBtn, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(-1);
        updateCalendar();
    });

    connect(nextBtn, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(1);
        updateCalendar();
    });

    topBarLayout->addWidget(prevBtn);
    topBarLayout->addStretch();
    topBarLayout->addLayout(dateLayout);
    topBarLayout->addStretch();
    topBarLayout->addWidget(nextBtn);

    calendarLayout->addLayout(topBarLayout);

    // --- Прокручиваемая область с днями недели ---
    QWidget* daysContainer = new QWidget(container);
    daysLayout = new QHBoxLayout(daysContainer);
    daysLayout->setContentsMargins(10, 0, 10, 0);
    daysLayout->setSpacing(0);

    // ------- Добавляем всё в общий вертикальный layout -------
    calendarLayout->addWidget(daysContainer);   // дни недели
    calendarLayout->setContentsMargins(0,0,0,0);
    calendarLayout->addStretch(0);

    // this->setLayout(calendarLayout); // или scrollArea->setWidget(container);
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins(0,0,0,0);

    this->setLayout(mainLayout);

    updateCalendar();
}

void CalendarWidget::updateCalendar() {
    QDate today = QDate::currentDate();
    QDate startDate = currentDate.addDays(-3); // 3 дня до currentDate

    QLocale locale = QLocale::system();

    // Заголовки месяца и года на основе currentDate
    this->monthLabel->setText(locale.toString(currentDate, "MMMM").toUpper());
    this->yearLabel->setText(locale.toString(currentDate, "yyyy").toUpper());

    // Очистка
    QLayoutItem* child;
    while ((child = daysLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    daysLayout->setContentsMargins(10, 0, 10, 0);
    daysLayout->setSpacing(10);

    for (int i = 0; i < 7; ++i) {
        QDate date = startDate.addDays(i);
        QString weekday = locale.toString(date, "ddd").toUpper();
        QString dayNumber = QString::number(date.day());

        QPushButton* dayButton = new QPushButton(QString("%1\n%2").arg(weekday, dayNumber), this);
        dayButton->setFixedSize(60, 70);

        QString style = "background-color: #1e1e1e; border-radius: 10px; font-weight: bold;";
        QString textColor = "color: gray;";

        if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
            textColor = "color: red;";
        }

        if (date == today) {
            style = "background-color: #887aff; border-radius: 10px; font-weight: bold;";
            textColor = (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) ? "color: red;" : "color: white;";
        }

        dayButton->setStyleSheet(style + textColor);
        dayButton->setFont(QFont("Segoe UI", 9));
        dayButton->setCursor(Qt::PointingHandCursor);

        daysLayout->addWidget(dayButton);
    }
}


void CalendarWidget::highlightToday(QPushButton* btn, const QDate& date)
{
    if (date == QDate::currentDate()) {
        btn->setStyleSheet(btn->styleSheet() +
                           " QPushButton { border: 2px solid #7a74ff; }");
    }
}

void CalendarWidget::highlightWeekend(QPushButton* btn, const QDate& date)
{
    if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
        btn->setStyleSheet(btn->styleSheet() +
                           " QPushButton { color: red; }");
    }
}
