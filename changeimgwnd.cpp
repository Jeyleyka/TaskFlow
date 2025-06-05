#include "changeimgwnd.h"

ChangeImgWnd::ChangeImgWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 311);

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

    this->importBtn = new QPushButton(tr("Import from gallery"), this);
    this->importBtn->setStyleSheet("background-color: #8182DE; height: 35px; border-radius: 5px;");

    this->closeBtn = new QPushButton(tr("Close"), this);
    this->closeBtn->setStyleSheet("background-color: transparent; height: 35px; color: #8182DE; border: 1px solid #8182DE; border-radius: 5px;");

    connect(this->closeBtn, &QPushButton::clicked, this, &ChangeImgWnd::close);

    connect(this->importBtn, &QPushButton::clicked, this, [this] {
        QString filePath = QFileDialog::getOpenFileName(this, "Select icon", "", "Images (*.png *.jpg *.jpeg *.bmp *.ico *.svg)");

        if (!filePath.isEmpty()) {
            QIcon icon(filePath);
            this->selectedIcon = icon;
            this->changeImgInDB();
        } else {
            QMessageBox::information(this, tr("No icon selected"), tr("Icon was not changed."));
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

    QMessageBox::warning(this, tr("Success"), tr("New img is saved"));

    emit this->onUpdateImg();
    this->close();
}
