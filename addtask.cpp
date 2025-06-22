#include "addtask.h"

AddTask::AddTask(QWidget* parent) : QWidget(parent), isClosing(false) {

    this->setFixedSize(375, 228);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: #121212; border-radius: 5px;");

    this->container = new QWidget(this);

    this->containerLayout = new QVBoxLayout(this->container);

    this->closeWndBtn = new QPushButton("×", this);
    this->closeWndBtn->setStyleSheet("background-color: transparent; border: none; font-size: 31px;");

    connect(this->closeWndBtn, &QPushButton::clicked, this, [this] {
        emit this->onCloseWnd();
        this->close();
    });

    this->wndTitle = new QLabel("Add Task", this);
    this->wndTitle->setStyleSheet("font-size: 17px; font-weight: 600; color: #fff");

    QHBoxLayout* headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(8);

    headerLayout->addWidget(this->wndTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(this->closeWndBtn);

    this->taskTitle = new QLabel("Title", this);
    this->taskTitle->setStyleSheet("font-size: 15px; color: #919191");
    this->taskTitle->installEventFilter(this);

    this->titleLineEdit = new QLineEdit(this);
    this->titleLineEdit->setStyleSheet("font-size: 17px; color: white; height: 43px; width: 325px; border: 1px solid #727272; border-radius: 5px; padding-left: 10px;");
    this->titleLineEdit->hide();

    connect(this->titleLineEdit, &QLineEdit::returnPressed, this, [this] {
        this->taskTitle->setText(this->titleLineEdit->text());
        this->titleLineEdit->hide();
        this->taskTitle->show();

        if (!this->descLineEdit->isHidden())
            this->titleLineEdit->hide();
    });

    this->description = new QLabel("Description", this);
    this->description->setStyleSheet("font-size: 15px; color: #919191");
    this->description->installEventFilter(this);

    this->descLineEdit = new QLineEdit(this);
    this->descLineEdit->setStyleSheet("font-size: 17px; color: white; height: 43px; width: 325px; border: 1px solid #727272; border-radius: 5px; padding-left: 10px;");
    this->descLineEdit->hide();

    connect(this->descLineEdit, &QLineEdit::returnPressed, this, [this] {
        this->description->setText(this->descLineEdit->text());
        this->descLineEdit->hide();
        this->description->show();

        if (!this->titleLineEdit->isHidden())
            this->descLineEdit->hide();
    });

    this->chooseDate = new QPushButton(this);
    this->chooseDate->setIcon(QIcon(":/icons/time.svg"));
    this->chooseDate->setIconSize(QSize(24,24));
    this->chooseDate->setStyleSheet("background-color: transparent; border: none");

    this->chooseCategory = new QPushButton(this);
    this->chooseCategory->setIcon(QIcon(":/icons/category.svg"));
    this->chooseCategory->setIconSize(QSize(24,24));
    this->chooseCategory->setStyleSheet("background-color: transparent; border: none");

    this->chooseCategoryWnd = new ChooseCategory(this);

    connect(this->chooseCategory, &QPushButton::clicked, this, [this] {
        this->chooseCategoryWnd->show();
    });

    this->choosePriority = new QPushButton(this);
    this->choosePriority->setIcon(QIcon(":/icons/priority.svg"));
    this->choosePriority->setIconSize(QSize(24,24));
    this->choosePriority->setStyleSheet("background-color: transparent; border: none");

    this->choosePriorityWnd = new ChoosePriority(this);

    connect(this->choosePriority, &QPushButton::clicked, this, [this] {
        this->choosePriorityWnd->show();
    });

    connect(this->chooseDate, &QPushButton::clicked, this, [this] {
        CustomCalendar* calendar = new CustomCalendar(this);
        calendar->show();

        connect(calendar, &CustomCalendar::dateSelected, this, [this](const QDate& date, int hour, int minutes, bool isAm) {
            this->date = date;
            this->hour = hour;
            this->minutes = minutes;
            this->isAm = isAm;

            if (!this->isAm)
                this->hour += 12;
        });
    });

    this->priorityWidget = new PriorityItemWidget(0, this);
    this->priorityWidget->hide();

    connect(this->choosePriorityWnd, &ChoosePriority::prioritySelected, this, [this](int priority) {
        this->priorityWidget = new PriorityItemWidget(priority, this);
        this->priorityWidget->setFixedSize(64,64);
        this->priorityWidget->hide();
    });

    this->itemWidget = new CategoryItemWidget("", QColor(), QIcon(""), this);
    this->itemWidget->hide();

    connect(this->chooseCategoryWnd, &ChooseCategory::categorySelected, this, [this](const QString& name, const QColor& color, const QIcon& icon) {
        if (name == "Create new")
            return;

        this->itemWidget = new CategoryItemWidget(name, color, icon, this);
        this->itemWidget->setFixedSize(64,64);
        this->itemWidget->hide();
    });

    this->addTaskBtn = new QPushButton(this);
    this->addTaskBtn->setIcon(QIcon(":/icons/addTask.svg"));
    this->addTaskBtn->setIconSize(QSize(24,24));
    this->addTaskBtn->setStyleSheet("background-color: transparent; border: none");

    connect(this->addTaskBtn, &QPushButton::clicked, this, [this] {
        if (this->titleLineEdit->text().isEmpty() || this->descLineEdit->text().isEmpty()
            || this->priorityWidget->getPriority() == 0 || this->itemWidget->getName() == "")
        {
            WarningWnd* warning = new WarningWnd(tr("Not all fields are recorded"), this);
            warning->showWithAnimation();
            return;
        }

        if (this->itemWidget)
            emit categorySelectedWidget(this->itemWidget);
        else
            qDebug() << "Widget is nullptr";

        emit this->onCreateTask();
    });

    this->buttonsLayout = new QHBoxLayout();
    this->buttonsLayout->addWidget(this->chooseDate);
    this->buttonsLayout->addWidget(this->chooseCategory);
    this->buttonsLayout->addWidget(this->choosePriority);
    this->buttonsLayout->addStretch();
    this->buttonsLayout->addWidget(this->addTaskBtn);

    this->containerLayout->addLayout(headerLayout);
    this->containerLayout->addWidget(this->taskTitle);
    this->containerLayout->addWidget(this->titleLineEdit);
    this->containerLayout->addWidget(this->description);
    this->containerLayout->addWidget(this->descLineEdit);
    this->containerLayout->addLayout(this->buttonsLayout);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(this->container);

    this->setLayout(this->mainLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

AddTask::~AddTask() {}

void AddTask::showCentered(QWidget* parent) {
    if (!parent) parent = this->parentWidget();

    if (parent) {
        QPoint center = parent->geometry().center();
        move(center.x() - width() / 2, center.y() - height() / 2);
    }

    this->show();
}

Task AddTask::getTask() const {
    Task t;
    t.title = this->titleLineEdit->text();
    t.description = this->descLineEdit->text();
    t.dueDate = QDateTime(this->date, QTime(this->hour, this->minutes));
    t.categoryName = this->itemWidget->getName();
    t.priority = this->priorityWidget->getPriority();
    t.categoryColor = this->itemWidget->getColor();
    t.categoryIcon = this->itemWidget->getIcon();
    return t;
}

CategoryItemWidget* AddTask::getSelectedCategoryWidget() {
    return this->itemWidget;
}

void AddTask::enterEditMode(QLabel* label, QLineEdit* editor) {
    label->hide();
    editor->setText(label->text());
    editor->show();
    editor->setFocus();
    editor->selectAll();
}

void AddTask::leaveEditMode(QLabel* label, QLineEdit* editor) {
    label->setText(editor->text());
    // emit textEdited(this->titleLineEdit->text());

    editor->hide();
    label->show();
}

void AddTask::focusOutEvent(QFocusEvent* event)  {
    QWidget::focusOutEvent(event);
    this->close();
}

bool AddTask::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == this->taskTitle) {
            // Скрыть description, если он редактируется
            if (this->descLineEdit->isVisible()) {
                this->descLineEdit->hide();
                this->description->show();
            }

            // Переключаем title в режим редактирования
            this->taskTitle->hide();
            this->titleLineEdit->setText(this->taskTitle->text());
            this->titleLineEdit->show();
            this->titleLineEdit->setFocus();
            return true;
        }

        if (obj == this->description) {
            // Скрыть title, если он редактируется
            if (this->titleLineEdit->isVisible()) {
                this->titleLineEdit->hide();
                this->taskTitle->show();
            }

            // Переключаем description в режим редактирования
            this->description->hide();
            this->descLineEdit->setText(this->description->text());
            this->descLineEdit->show();
            this->descLineEdit->setFocus();
            return true;
        }

        if (event->type() == QEvent::MouseButtonPress) {
            if (obj != this->taskTitle && obj != this->titleLineEdit && this->titleLineEdit->isVisible()) {
                this->titleLineEdit->hide();
                this->taskTitle->show();
            }

            if (obj != this->description && obj != this->descLineEdit && this->descLineEdit->isVisible()) {
                this->descLineEdit->hide();
                this->description->show();
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void AddTask::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        event->accept(); // блокируем стандартное поведение
        return;
    }
    QWidget::keyPressEvent(event);
}
