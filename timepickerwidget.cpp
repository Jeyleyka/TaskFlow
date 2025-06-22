#include "TimePickerWidget.h"

ScrollSelector::ScrollSelector(const QStringList& values, QWidget* parent)
    : QWidget(parent), items(values), current(0), startY(0), offsetY(0), isDragging(false) {

    this->setFixedSize(64, 64);
    this->setStyleSheet("background-color: #272727; border-radius: 5px;");

    // Главный layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Контейнер с layout
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);  // Обязательно убрать отступы
    layout->setAlignment(Qt::AlignCenter);

    prevLabel = new QLabel("", this);
    currentLabel = new QLabel("", this);
    nextLabel = new QLabel("", this);

    for (QLabel* label : {prevLabel, currentLabel, nextLabel}) {
        label->setAlignment(Qt::AlignCenter);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // растягиваться
        label->setMinimumHeight(20);
    }

    currentLabel->setStyleSheet("color: #8687E7; font-size: 24px;");
    prevLabel->setStyleSheet("color: gray; font-size: 16px;");
    nextLabel->setStyleSheet("color: gray; font-size: 16px;");

    layout->addWidget(prevLabel);
    layout->addWidget(currentLabel);
    layout->addWidget(nextLabel);

    container->setLayout(layout);
    mainLayout->addWidget(container);
    this->setLayout(mainLayout);

    setCurrentIndex(0);
    currentLabel->installEventFilter(this);
}

void ScrollSelector::setCurrentIndex(int index) {
    current = (index + items.size()) % items.size();
    updateLabels();
    emit valueChanged(current);
}

int ScrollSelector::currentIndex() const {
    return current;
}

QString ScrollSelector::currentText() const {
    return items[current];
}

void ScrollSelector::updateLabels() {
    currentLabel->setText(items.value(current, ""));
    prevLabel->setText(items.value(current - 1, ""));
    nextLabel->setText(items.value(current + 1, ""));
}

bool ScrollSelector::eventFilter(QObject* obj, QEvent* event) {
    if (obj == currentLabel && event->type() == QEvent::MouseButtonPress) {
        setCurrentIndex(current + 1);
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void ScrollSelector::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        this->startY = event->pos().y();
        this->isDragging = true;
    }
}

void ScrollSelector::mouseMoveEvent(QMouseEvent *event) {
    if (this->isDragging) {
        int deltaY = event->pos().y() - this->startY;
        this->offsetY = deltaY;

        if (deltaY > 30 && this->current > 0) {
            this->current--;
            this->updateLabels();
            this->startY = event->pos().y();
        } else if (deltaY < -30 && this->current < this->items.size() - 1) {
            this->current++;
            this->updateLabels();
            this->startY = event->pos().y();
        }
    }
}

void ScrollSelector::mouseReleaseEvent(QMouseEvent*) {
    isDragging = false;
    offsetY = 0;
}

TimePickerWidget::TimePickerWidget(const QDate& date, QWidget* parent)
    : QDialog(parent), date(date) {
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(327, 206);
    this->setStyleSheet("background-color: #2e2e2e; border-radius: 5px;");

    QWidget* container = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel* title = new QLabel("Choose Time");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 18px;");
    mainLayout->addWidget(title);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #555;");
    mainLayout->addWidget(line);

    QHBoxLayout* timeLayout = new QHBoxLayout();
    timeLayout->setSpacing(10);
    timeLayout->setAlignment(Qt::AlignCenter);

    QStringList hours;
    for (int i = 1; i <= 12; ++i) hours << QString::number(i);

    QStringList minutes;
    for (int i = 0; i < 60; ++i) minutes << QString("%1").arg(i, 2, 10, QLatin1Char('0'));

    QStringList ampm = {"AM", "PM"};

    hourSelector = new ScrollSelector(hours);
    minuteSelector = new ScrollSelector(minutes);
    ampmSelector = new ScrollSelector(ampm);

    QLabel* colon = new QLabel(":");
    colon->setStyleSheet("color: white; font-size: 20px;");
    colon->setFixedWidth(10);
    colon->setAlignment(Qt::AlignCenter);

    timeLayout->addWidget(hourSelector);
    timeLayout->addWidget(colon);
    timeLayout->addWidget(minuteSelector);
    timeLayout->addSpacing(15);
    timeLayout->addWidget(ampmSelector);

    mainLayout->addLayout(timeLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* chooseBtn = new QPushButton("Choose");
    chooseBtn->setEnabled(true);

    cancelBtn->setFixedSize(140, 48);
    chooseBtn->setFixedSize(140, 48);

    cancelBtn->setStyleSheet("background-color: transparent; border: none; color: #6F70BF;");
    chooseBtn->setStyleSheet("background-color: #8687E7; color: white;");

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(chooseBtn);

    connect(cancelBtn, &QPushButton::clicked, this, &TimePickerWidget::close);
    connect(chooseBtn, &QPushButton::clicked, this, [=]() {
        emit timeChosen(
            this->date,
            hourSelector->currentText().toInt(),
            minuteSelector->currentText().toInt(),
            ampmSelector->currentText() == "AM"
            );
        this->close();
    });

    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(container);
    layout->setContentsMargins(0,0,0,0);

    this->setLayout(layout);
}
