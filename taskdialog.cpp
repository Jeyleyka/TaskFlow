#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent), isClosing(false) {
    this->setWindowTitle("Add task");

    this->titleLineEdit = new QLineEdit(this);

    this->descriptionTextEdit = new QTextEdit(this);

    this->dueDateEdit = new QDateEdit(QDate::currentDate(), this);
    this->dueDateEdit->setCalendarPopup(true);

    this->priorityComboBox = new QComboBox(this);
    this->priorityComboBox->addItems({"Low", "Medium", "High"});

    this->statusComboBox = new QComboBox(this);
    this->statusComboBox->addItems({"Todo", "In Progress", "Done"});

    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(this->buttonBox, &QDialogButtonBox::accepted, this, [this] {
        if (this->titleLineEdit->text().isEmpty() || this->descriptionTextEdit->toPlainText().isEmpty())
        {
            QMessageBox::warning(this, "Error", "the area must not be empty");
            return;
        }

        QDialog::accept();
    });

    connect(this->buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::hideWithAnim);

    this->formLayout = new QFormLayout;
    this->formLayout->addRow("Title:", this->titleLineEdit);
    this->formLayout->addRow("Description:", this->descriptionTextEdit);
    this->formLayout->addRow("Due Date:", this->dueDateEdit);
    this->formLayout->addRow("Priority:", this->priorityComboBox);
    this->formLayout->addRow("Status:", this->statusComboBox);

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
    t.priority = this->priorityComboBox->currentText();
    t.status = this->statusComboBox->currentText();
    return t;
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
