// CustomCalendar.cpp

#include "CustomCalendar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QFrame>

CustomCalendar::CustomCalendar(QWidget* parent)
    : QWidget(parent), currentDate(QDate::currentDate()) {
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(327, 326);
    this->setStyleSheet("background-color: #2e2e2e; border-radius: 5px;");

    QWidget* container = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Header (navigation and month)
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QPushButton* prevBtn = new QPushButton("<");
    QPushButton* nextBtn = new QPushButton(">");

    this->monthLabel = new QLabel();
    this->monthLabel->setStyleSheet("color: white; font-size: 16px;");
    this->monthLabel->setAlignment(Qt::AlignCenter);

    this->yearLabel = new QLabel();
    this->yearLabel->setStyleSheet("color: #727272; font-size: 14px;");
    this->yearLabel->setAlignment(Qt::AlignCenter);

    auto* dateLayout = new QVBoxLayout;
    dateLayout->addWidget(this->monthLabel);
    dateLayout->setSpacing(0);
    dateLayout->addWidget(this->yearLabel);
    dateLayout->setContentsMargins(0,0,0,0);

    for (auto btn : {prevBtn, nextBtn}) {
        btn->setFixedSize(32, 32);
        btn->setStyleSheet("color: white; background-color: transparent; font-size: 19px; border: none;");
    }

    headerLayout->addWidget(prevBtn);
    headerLayout->addStretch();
    headerLayout->addLayout(dateLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(nextBtn);

    connect(prevBtn, &QPushButton::clicked, this, &CustomCalendar::onPrevMonth);
    connect(nextBtn, &QPushButton::clicked, this, &CustomCalendar::onNextMonth);

    mainLayout->addLayout(headerLayout);

    // Divider
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #555;");
    mainLayout->addWidget(line);

    // Weekdays
    QHBoxLayout* weekdaysLayout = new QHBoxLayout();
    QStringList weekdays;
    QLocale locale;

    for (int i = 0; i < 6; ++i)
        weekdays << locale.standaloneDayName(i, QLocale::ShortFormat);

    for (int i = 1; i <= 7; ++i) {
        QLabel* label = new QLabel(locale.standaloneDayName(i, QLocale::ShortFormat));
        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(21, 15);
        if (i == 6 || i == 7) { // суббота или воскресенье
            label->setStyleSheet("color: #FF4949; font-size: 13px;");
        } else {
            label->setStyleSheet("color: white; font-size: 13px;");
        }
        weekdaysLayout->addWidget(label);
    }

    mainLayout->addLayout(weekdaysLayout);

    // Days grid
    daysLayout = new QGridLayout();
    daysLayout->setSpacing(4);
    mainLayout->addLayout(daysLayout);

    // Buttons
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* chooseBtn = new QPushButton("Choose Time");

    cancelBtn->setStyleSheet("background-color: transparent; border: none; color: #6F70BF;");
    cancelBtn->setFixedSize(153, 48);

    chooseBtn->setStyleSheet("background-color: #8687E7; color: white;");
    chooseBtn->setFixedSize(153, 48);

    bottomLayout->addWidget(cancelBtn);
    bottomLayout->addStretch();
    bottomLayout->addWidget(chooseBtn);

    connect(cancelBtn, &QPushButton::clicked, this, [this] {
        emit this->closeCalendar();
        this->close();
    });
    connect(chooseBtn, &QPushButton::clicked, this, [this]() {
        TimePickerWidget* timeWidget = new TimePickerWidget(this->currentDate, this);
        timeWidget->show();

        connect(timeWidget, &TimePickerWidget::timeChosen, this, [this](QDate& date, int hour, int minutes, bool isAm) {
            emit dateSelected(date, hour, minutes, isAm);
        });

        this->close();
    });

    mainLayout->addLayout(bottomLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(container);
    layout->setContentsMargins(0,0,0,0);

    this->setLayout(layout);

    updateCalendar();
}

void CustomCalendar::showCentered(QWidget* parent) {
    if (!parent) parent = this->parentWidget();

    if (parent) {
        QPoint center = parent->geometry().center();
        move(center.x() - width() / 2, center.y() - height() / 2);
    }

    this->show();
}

void CustomCalendar::onPrevMonth() {
    currentDate = currentDate.addMonths(-1);
    updateCalendar();
}

void CustomCalendar::onNextMonth() {
    currentDate = currentDate.addMonths(1);
    updateCalendar();
}

void CustomCalendar::clearCalendar() {
    for (QPushButton* btn : dayButtons) {
        daysLayout->removeWidget(btn);
        btn->deleteLater();
    }
    dayButtons.clear();
}

void CustomCalendar::updateCalendar() {
    clearCalendar();
    QDate firstDay(currentDate.year(), currentDate.month(), 1);
    int startColumn = firstDay.dayOfWeek() % 7;  // Make Monday = 0
    int daysInMonth = firstDay.daysInMonth();

    QLocale locale = QLocale::system();

    this->monthLabel->setText(locale.toString(currentDate, "MMMM").toUpper());
    this->yearLabel->setText(locale.toString(currentDate, "yyyy").toUpper());

    int row = 0;
    for (int i = 1; i <= daysInMonth; ++i) {
        QPushButton* dayBtn = new QPushButton(QString::number(i));
        dayBtn->setFixedSize(24, 24);

        if (i == currentDate.day() && QDate::currentDate().month() == currentDate.month())
            dayBtn->setStyleSheet("background-color: #8687E7; color: white; border-radius: 4px;");
        else
            dayBtn->setStyleSheet("background-color: #272727; color: white; border-radius: 4px;");

        dayButtons.append(dayBtn);
        int col = (startColumn + i - 1) % 7;
        row = (startColumn + i - 1) / 7;

        daysLayout->addWidget(dayBtn, row, col);
        connect(dayBtn, &QPushButton::clicked, this, [this, i]() {
            currentDate.setDate(currentDate.year(), currentDate.month(), i);
            updateCalendar();
        });
    }
}
