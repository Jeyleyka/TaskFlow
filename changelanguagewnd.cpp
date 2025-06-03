#include "changelanguagewnd.h"

ChangeLanguageWnd::ChangeLanguageWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setFixedSize(375, 500);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    this->wndTitle = new QLabel(tr("Change app language"), this);
    this->wndTitle->setStyleSheet("font-size: 17px; color: #fff; margin-bottom: 15px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->languages = new QComboBox(this);
    this->languages->setStyleSheet(R"(
    QComboBox {
        margin-top: 15px;
        border: 1px solid #8687E7;
        border-radius: 8px;
        padding: 6px 12px;
        background-color: #363636;
        color: white;
        font-size: 14px;
    }

    QComboBox:hover {
        border: 1px solid #5A5ADC;
    }

    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: top right;
        width: 25px;
        border-left: 1px solid #8687E7;
    }

    QComboBox::down-arrow {
        image: url(:/icons/down-arrow.png); /* или убери, если не используешь */
        width: 14px;
        height: 14px;
    }

    QComboBox QAbstractItemView {
        background-color: #363636;
        border: 1px solid #8687E7;
        selection-background-color: #36362D;
        selection-color: white;
        padding: 4px;
        font-size: 14px;
        outline: 0;
    }

    QComboBox QAbstractItemView::item {
        padding: 6px 12px;
    }

    QComboBox QAbstractItemView::item:hover {
        background-color: #C9C9C9;
    }
)");

    this->languages->addItem("English");
    this->languages->addItem("Русский");
    this->languages->addItem("Español");
    this->languages->addItem("Deutsch");
    this->languages->addItem("Français");
    this->languages->addItem("Português");

    this->warning = new QLabel(tr("After selecting the language the application will restart"));
    this->warning->setWordWrap(true);
    this->warning->setStyleSheet("font-size: 15px; color: #fff");
    this->warning->setAlignment(Qt::AlignHCenter);

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 130px; height: 48px; color: #7C7DD1; background-color: transparent; font-size: 13px; border: none;");

    connect(this->cancel, &QPushButton::clicked, this, &ChangeLanguageWnd::close);

    this->btnsLayout = new QHBoxLayout;
    this->btnsLayout->setContentsMargins(15,15,15,15);

    this->btnsLayout->addWidget(this->cancel);

    this->select = new QPushButton(tr("Select"), this);
    this->select->setStyleSheet("width: 130px; height: 48px; color: #fff; background-color: #8687E7; font-size: 13px;");

    connect(this->select, &QPushButton::clicked, this, [this] {
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("language", this->languages->currentText());

        QString program = QCoreApplication::applicationFilePath();
        QStringList arguments = QCoreApplication::arguments();

        QProcess::startDetached(program, arguments);
        QCoreApplication::quit();
    });

    this->btnsLayout->addWidget(this->select);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(0,0,0,0);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);
    this->mainLayout->addWidget(this->languages);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->warning);
    this->mainLayout->addStretch();
    this->mainLayout->addLayout(this->btnsLayout);

    this->setLayout(this->mainLayout);
}
