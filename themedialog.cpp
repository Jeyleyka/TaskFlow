#include "themedialog.h"

ThemeDialog::ThemeDialog(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    this->mainLayout = new QVBoxLayout(this);

    this->addColorOption("background", "Background", ThemeManager::instance().backgroundColor());
    this->addColorOption("widgets", "Widgets", ThemeManager::instance().widgetsColor());
    this->addColorOption("navbar", "Navbar", ThemeManager::instance().navbarColor());
    this->addColorOption("button", "Button", ThemeManager::instance().buttonColor());

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton* resetBtn = new QPushButton("Reset");
    buttonBox->addButton(resetBtn, QDialogButtonBox::ResetRole);

    connect(resetBtn, &QPushButton::clicked, this, [this] {
        ThemeManager::instance().resetToDefault();
        this->accept();
    });

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        this->applyTheme();
        this->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, &ThemeDialog::reject);

    this->mainLayout->addWidget(buttonBox);
}

ThemeDialog::~ThemeDialog() = default;

void ThemeDialog::addColorOption(const QString &key, const QString &labelText, const QColor &defaultColor) {
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label = new QLabel(labelText);
    QPushButton* button = new QPushButton("Choose...");
    QLabel* preview = new QLabel;
    preview->setFixedSize(40,20);
    preview->setStyleSheet("background-color: " + defaultColor.name() + "; border: 1px solid black");

    connect(button, &QPushButton::clicked, this, [this, defaultColor, preview, key] {
        QColor chosen = QColorDialog::getColor(defaultColor, this, "Choose color");

        if (chosen.isValid()) {
            this->selectedColors[key] = chosen;
            preview->setStyleSheet("background-color: " + chosen.name() + "; border: 1px; solid black");
        }
    });

    this->selectedColors[key] = defaultColor;

    layout->addWidget(label);
    layout->addWidget(preview);
    layout->addWidget(button);
    layout->addStretch();

    this->colorItems[key] = {button, preview};

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(this->layout());

    mainLayout->addLayout(layout);
}

void ThemeDialog::applyTheme() {
    auto& theme = ThemeManager::instance();

    theme.setBackgroundColor(this->selectedColors["background"]);
    theme.setWidgetsColor(this->selectedColors["widgets"]);
    theme.setNavbarColor(this->selectedColors["navbar"]);
    theme.setButtonColor(this->selectedColors["button"]);

    theme.saveTheme();
}
