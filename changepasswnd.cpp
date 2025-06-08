#include "changepasswnd.h"

ChangePass::ChangePass(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 311);
    this->setStyleSheet("background-color: #121212;");

    // this->container = new QWidget(parent);

    this->wndTitle = new QLabel(tr("Change account Password"), this);
    this->wndTitle->setStyleSheet("font-size: 16px; margin-bottom: 6px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->oldPassword = new QLabel(tr("Enter old password"), this);
    this->oldPassword->setStyleSheet("font-size: 14px; color: #fff");

    this->oldPasswordEdit = new QLineEdit(this);
    this->oldPasswordEdit->setPlaceholderText(tr("Enter old password"));
    this->oldPasswordEdit->setEchoMode(QLineEdit::Password);
    this->oldPasswordEdit->setStyleSheet("background-color: transparent; border: 1px solid #727272; font-size: 15px; width: 287px; height: 43px; margin-top: 10px;");

    this->newPassword = new QLabel(tr("Enter new password"), this);
    this->newPassword->setStyleSheet("font-size: 14px; color: #fff");

    this->newPasswordEdit = new QLineEdit(this);
    this->newPasswordEdit->setPlaceholderText(tr("Enter new password"));
    this->newPasswordEdit->setEchoMode(QLineEdit::Password);
    this->newPasswordEdit->setStyleSheet("background-color: transparent; border: 1px solid #727272; font-size: 15px; width: 287px; height: 43px; margin-top: 10px;");

    ThemeManager::instance().loadTheme();

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: #7C7DD1");

    QColor color = ThemeManager::instance().buttonColor();
    this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: " + color.name());
    });

    connect(this->cancel, &QPushButton::clicked, this, &ChangePass::close);

    this->edit = new QPushButton(tr("Edit"), this);
    this->edit->setStyleSheet("width: 153px; height: 48px; background-color: #8687E7; color: #fff");

    this->edit->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->edit->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: " + color.name());
    });

    connect(this->edit, &QPushButton::clicked, this, [this] {
        if (this->newPasswordEdit->text().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("New password must not be empty!"));
            return;
        }

        this->changePassInDB();
    });

    this->btnsLayout = new QHBoxLayout();
    this->btnsLayout->addWidget(this->cancel);
    this->btnsLayout->addWidget(this->edit);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);
    this->mainLayout->addWidget(this->oldPassword);
    this->mainLayout->addWidget(this->oldPasswordEdit);
    this->mainLayout->addWidget(this->newPassword);
    this->mainLayout->addWidget(this->newPasswordEdit);
    this->mainLayout->addStretch();
    this->mainLayout->addLayout(this->btnsLayout);

    this->setLayout(this->mainLayout);
}

ChangePass::~ChangePass() {}

void ChangePass::changePassInDB() {
    QSqlQuery query;

    query.prepare("SELECT password FROM users WHERE id = :id");
    query.bindValue(":id", UserSession::getUserId());

    if (query.exec()) {
        if (query.next()) {
            this->oldPass = query.value("password").toString();
        }
    }

    if (this->oldPasswordEdit->text().trimmed() == this->oldPass.trimmed())
    {
        if (this->newPasswordEdit->text() != this->oldPass.trimmed())
        {
            query.prepare("UPDATE user SET password = :password WHERE id = :id");

            query.bindValue(":id", UserSession::getUserId());
            query.bindValue(":password", this->newPasswordEdit->text());
        } else
        {
            QMessageBox::warning(this, tr("Warning"), tr("The new password matches the current password"));
            return;
        }


        if (!query.exec()) {
            qDebug() << "Ошибка при обновлении задачи:" << query.lastError().text();
        }
    } else
    {
        QMessageBox::warning(this, tr("Error"), tr("old password is incorrect"));
        return;
    }

    QMessageBox::warning(this, tr("Success"), tr("New password is saved"));
    this->close();
}
