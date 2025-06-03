#include "categoryeditor.h"

CategoryEditor::CategoryEditor(QWidget* parent)
    : QDialog(parent) {
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 500);

    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(R"(
        #container {
            background-color: #2e2e2e;
            border-radius: 15px;
        }
    )");

    QVBoxLayout* containerLayout = new QVBoxLayout(container);

    this->wndTitle = new QLabel(tr("Create new category"), this);
    this->wndTitle->setStyleSheet("font-size: 17px; color: #fff; font-weight: 500;");

    containerLayout->addWidget(this->wndTitle);

    this->categoryName = new QLabel(tr("Category name:"), this);
    this->categoryName->setStyleSheet("font-size: 14px; color: #fff; margin-top: 10px;");

    containerLayout->addWidget(this->categoryName);

    this->nameEdit = new QLineEdit(this);
    this->nameEdit->setPlaceholderText(tr("Category name"));
    this->nameEdit->setStyleSheet("border: 1px solid #727272; width: 327px; height: 48px; margin-top: 10px;");

    containerLayout->addWidget(this->nameEdit);

    this->colorButton = new QPushButton(tr("Choose color"), this);

    connect(this->colorButton, &QPushButton::clicked, this, &CategoryEditor::chooseColor);

    containerLayout->addWidget(this->colorButton);

    this->categoryIcon = new QLabel(tr("Category icon:"), this);
    this->categoryIcon->setStyleSheet("font-size: 14px; color: #fff; margin-top: 10px;");

    containerLayout->addWidget(this->categoryIcon);

    this->iconCombo = new QComboBox(this);
    this->iconCombo->addItem(QIcon(":/icons/briefcase.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/dumbbell.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/healthcare.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/home.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/megaphone.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/music.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/ux-design.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/video-camera.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/graduation-cap.png"), "");
    this->iconCombo->addItem(QIcon(":/icons/plus.png"), "");

    containerLayout->addWidget(iconCombo);

    this->btnsLayout = new QHBoxLayout(container);

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 153px; height: 48px; color: #7C7DD1; background-color: transparent; font-size: 13px; border: none;");

    connect(this->cancel, &QPushButton::clicked, this, &CategoryEditor::close);

    this->btnsLayout->addWidget(this->cancel);

    this->createCategory = new QPushButton(tr("Create Category"), this);
    this->createCategory->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: #8687E7; font-size: 13px;");

    connect(this->createCategory, &QPushButton::clicked, this, &CategoryEditor::onCreateCategoryClicked);

    this->btnsLayout->addWidget(this->createCategory);
    containerLayout->addStretch();

    containerLayout->addLayout(this->btnsLayout, 5);


    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(container);
    this->mainLayout->setContentsMargins(0, 0, 0, 0);
}

void CategoryEditor::onCreateCategoryClicked() {
    QString name = this->nameEdit->text();
    QColor color = this->selectedColor;
    QIcon icon = this->iconCombo->currentData(Qt::DecorationRole).value<QIcon>();

    emit categoryCreated(name, color, icon);
    this->close();
}

void CategoryEditor::chooseColor() {
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid()) {
        this->selectedColor = color.name();
        this->colorButton->setStyleSheet("background-color: " + this->selectedColor);
    }
}

void CategoryEditor::saveCategory() {
    if (this->nameEdit->text().trimmed().isEmpty()) {
        qDebug() << "Name can't be empty";
        return;
    }

    this->accept();
}
