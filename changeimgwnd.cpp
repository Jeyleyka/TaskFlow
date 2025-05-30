#include "changeimgwnd.h"

ChangeImgWnd::ChangeImgWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 311);

    this->mainLayout = new QVBoxLayout(this);

    this->wndTitle = new QLabel("Change account icon", this);
    this->wndTitle->setStyleSheet("font-size: 16px; margin-bottom: 6px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);

    this->importBtn = new QPushButton("Import from gallery", this);
    this->importBtn->setStyleSheet("background-color: transparent; border: none;");

    connect(this->importBtn, &QPushButton::clicked, this, [this] {
        QString filePath = QFileDialog::getOpenFileName(this, "Select icon", "", "Images (*.png *.jpg *.jpeg *.bmp *.ico *.svg)");

        if (!filePath.isEmpty()) {
            QIcon icon(filePath);
            this->selectedIcon = icon;
        }

        this->changeImgInDB();
    });

    this->mainLayout->addWidget(this->importBtn);

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

    QMessageBox::warning(this, "Success", "New img is saved");

    emit this->onUpdateImg();
    this->close();
}
