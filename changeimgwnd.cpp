#include "changeimgwnd.h"

ChangeImgWnd::ChangeImgWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 311);
    this->setStyleSheet("background-color: #121212;");

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(15,15,15,15);

    this->wndTitle = new QLabel(tr("Change account icon"), this);
    this->wndTitle->setStyleSheet("font-size: 16px; margin-bottom: 6px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->mainLayout->addWidget(this->wndTitle, 0, Qt::AlignTop);
    this->mainLayout->addWidget(this->line);

    ThemeManager::instance().loadTheme();

    this->importBtn = new QPushButton(tr("Import from gallery"), this);
    this->importBtn->setStyleSheet("background-color: #8182DE; height: 35px; border-radius: 5px;");

    QColor color = ThemeManager::instance().buttonColor();
    this->importBtn->setStyleSheet("height: 35px; border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->importBtn->setStyleSheet("height: 35px; border-radius: 5px; background-color: " + color.name());
    });

    this->closeBtn = new QPushButton(tr("Close"), this);
    this->closeBtn->setStyleSheet("background-color: transparent; height: 35px; color: #8182DE; border: 1px solid #8182DE; border-radius: 5px;");

    QString style = QString("background-color: transparent; height: 35px; color: %1; border: 1px solid %1; border-radius: 5px;").arg(color.name());
    this->closeBtn->setStyleSheet(style);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        QString style = QString("background-color: transparent; height: 35px; color: %1; border: 1px solid %1; border-radius: 5px;").arg(color.name());
        this->closeBtn->setStyleSheet(style);
    });

    connect(this->closeBtn, &QPushButton::clicked, this, &ChangeImgWnd::close);

    connect(this->importBtn, &QPushButton::clicked, this, [this] {
        QString filePath = QFileDialog::getOpenFileName(this, "Select icon", "", "Images (*.png *.jpg *.jpeg *.bmp *.ico *.svg)");

        if (!filePath.isEmpty()) {
            QIcon icon(filePath);
            this->selectedIcon = icon;
            this->changeImgInDB();
        } else {
            WarningWnd* warning = new WarningWnd(tr("Icon was not changed."), this);
            warning->showWithAnimation();
        }
    });

    this->mainLayout->addWidget(this->importBtn);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->closeBtn);

    this->setLayout(this->mainLayout);
}

ChangeImgWnd::~ChangeImgWnd() {}

void ChangeImgWnd::changeImgInDB() {
    QSqlQuery query;

    query.prepare("UPDATE users SET icon = :icon WHERE id = :id");
    query.bindValue(":id", UserSession::getUserId());

    QByteArray iconBytes;
    QPixmap pixmap = this->selectedIcon.pixmap(54, 54);
    QBuffer buffer(&iconBytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    query.bindValue(":icon", iconBytes);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении задачи:" << query.lastError().text();
    }

    WarningWnd* warning = new WarningWnd(tr("New img is saved"), this);
    warning->showWithAnimation();


    emit this->onUpdateImg();
    this->close();
}
