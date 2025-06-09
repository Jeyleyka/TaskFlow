#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent), isClosing(false) {
    this->setWindowTitle("Add task");

    this->titleLineEdit = new QLineEdit(this);

    this->descriptionTextEdit = new QTextEdit(this);

    this->dueDateEdit = new QDateEdit(QDate::currentDate(), this);
    this->dueDateEdit->setCalendarPopup(true);

    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(this->buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::hideWithAnim);

    this->chooseCategory = new QPushButton(tr("Choose category"), this);
    this->chooseCategory->setStyleSheet("max-width: 120px; height: 25px; background-color: #2d2d2d");

    this->chooseCategoryWnd = new ChooseCategory(this);

    connect(this->chooseCategory, &QPushButton::clicked, this, [this] {
        this->chooseCategoryWnd->show();
    });

    this->choosePriority = new QPushButton(tr("Choose priority"), this);
    this->choosePriority->setStyleSheet("max-width: 120px; height: 25px; background-color: #2d2d2d");

    this->choosePriorityWnd = new ChoosePriority(this);

    connect(this->choosePriority, &QPushButton::clicked, this, [this] {
        this->choosePriorityWnd->show();
    });

    this->formLayout = new QFormLayout;
    this->formLayout->addRow(tr("Title:"), this->titleLineEdit);
    this->formLayout->addRow(tr("Description:"), this->descriptionTextEdit);
    this->formLayout->addRow(tr("Due Date:"), this->dueDateEdit);
    this->formLayout->addRow(tr("Priority: "), this->choosePriority);
    this->formLayout->addRow(tr("Category: "), this->chooseCategory);

    this->priorityWidget = new PriorityItemWidget(0, this);
    this->priorityWidget->hide();

    connect(this->choosePriorityWnd, &ChoosePriority::prioritySelected, this, [this](int priority) {
        this->priorityWidget = new PriorityItemWidget(priority, this);
        this->priorityWidget->setFixedSize(64,64);
        this->priorityWidget->show();

        this->formLayout->removeRow(3);
        this->formLayout->addRow(tr("Priority: "), priorityWidget);
    });

    qDebug() << "priority: " << this->priorityWidget->getPriority();

    this->itemWidget = new CategoryItemWidget("", QColor(), QIcon(""), this);
    this->itemWidget->hide();

    connect(this->chooseCategoryWnd, &ChooseCategory::categorySelected, this, [this](const QString& name, const QColor& color, const QIcon& icon) {
        if (name == "Create new")
            return;

        this->itemWidget = new CategoryItemWidget(name, color, icon, this);
        this->itemWidget->setFixedSize(64,64);
        this->itemWidget->show();

        this->formLayout->removeRow(3);
        this->formLayout->addRow(tr("Category: "), this->itemWidget);
    });

    connect(this->buttonBox, &QDialogButtonBox::accepted, this, [this] {
        if (this->titleLineEdit->text().isEmpty() || this->descriptionTextEdit->toPlainText().isEmpty()
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

        QDialog::accept();
    });

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addLayout(this->formLayout);
    this->mainLayout->addWidget(this->buttonBox);

    this->setLayout(this->mainLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

TaskDialog::~TaskDialog() {}

Task TaskDialog::getTask() const {
    Task t;
    t.title = this->titleLineEdit->text();
    t.description = this->descriptionTextEdit->toPlainText();
    t.dueDate = QDateTime(this->dueDateEdit->date(), QTime::currentTime());
    t.categoryName = this->itemWidget->getName();
    t.priority = this->priorityWidget->getPriority();
    t.categoryColor = this->itemWidget->getColor();
    t.categoryIcon = this->itemWidget->getIcon();
    return t;
}

CategoryItemWidget* TaskDialog::getSelectedCategoryWidget() {
    if (this->itemWidget)
        return this->itemWidget;
}

void TaskDialog::hideWithAnim() {
    QPoint startPos = pos();
    QPoint endPos(startPos.x(), startPos.y() + this->height());

    this->isClosing = true;

    QPropertyAnimation* hide = new QPropertyAnimation(this, "pos");
    hide->setDuration(600);
    hide->setStartValue(startPos);
    hide->setEndValue(endPos);
    hide->setEasingCurve(QEasingCurve::OutCubic);

    connect(hide, &QPropertyAnimation::finished, this, [this] {
        this->close();
    });

    hide->start(QAbstractAnimation::DeleteWhenStopped);

    // QDialog::reject();
}

void TaskDialog::closeEvent(QCloseEvent *event) {
    if (!this->isClosing)
    {
        event->ignore();
        this->hideWithAnim();
    } else {
        event->accept();
    }
}
