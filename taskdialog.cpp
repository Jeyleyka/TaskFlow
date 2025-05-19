#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent), isClosing(false) {
    this->setWindowTitle("Add task");

    this->titleLineEdit = new QLineEdit(this);

    this->descriptionTextEdit = new QTextEdit(this);

    this->dueDateEdit = new QDateEdit(QDate::currentDate(), this);
    this->dueDateEdit->setCalendarPopup(true);

    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(this->buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::hideWithAnim);

    this->chooseCategory = new QPushButton("Choose category", this);
    this->chooseCategory->setStyleSheet("max-width: 100px; height: 25px; background-color: #2d2d2d");

    this->chooseCategoryWnd = new ChooseCategory(this);

    connect(this->chooseCategory, &QPushButton::clicked, this, [this] {
        this->chooseCategoryWnd->show();
    });

    this->formLayout = new QFormLayout;
    this->formLayout->addRow("Title:", this->titleLineEdit);
    this->formLayout->addRow("Description:", this->descriptionTextEdit);
    this->formLayout->addRow("Due Date:", this->dueDateEdit);
    this->formLayout->addRow("Category: ", this->chooseCategory);


    connect(this->chooseCategoryWnd, &ChooseCategory::categorySelected, this, [this](const QString& name, const QColor& color, const QIcon& icon) {
        this->itemWidget = new CategoryItemWidget(name, color, icon, this);
        this->itemWidget->setFixedSize(64,64);

        this->formLayout->removeRow(3);
        this->formLayout->addRow("Category: ", this->itemWidget);
    });

    connect(this->buttonBox, &QDialogButtonBox::accepted, this, [this] {
        if (this->titleLineEdit->text().isEmpty() || this->descriptionTextEdit->toPlainText().isEmpty())
        {
            QMessageBox::warning(this, "Error", "the area must not be empty");
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
    t.categoryColor = this->itemWidget->getColor();
    t.categoryIcon = this->itemWidget->getIcon();
    return t;
}

CategoryItemWidget* TaskDialog::getSelectedCategoryWidget() {
    if (this->itemWidget)
        return this->itemWidget;
    else
    {
        QMessageBox::warning(this, "Error", "widget is invalid");
        return this->itemWidget;
    }
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
