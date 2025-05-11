#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent) {
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

    connect(this->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(this->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

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
}

TaskDialog::~TaskDialog() {}

Task TaskDialog::getTask() const {
    Task t;
    t.title = this->titleLineEdit->text();
    t.description = this->descriptionTextEdit->toPlainText();
    t.dueDate = this->dueDateEdit->date();
    t.priority = this->priorityComboBox->currentText();
    t.status = this->statusComboBox->currentText();
    return t;
}
