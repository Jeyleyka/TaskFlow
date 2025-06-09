#include "changeaccountnamewnd.h"

ChangeAccountNameWnd::ChangeAccountNameWnd(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 187);
    this->setStyleSheet("background-color: #121212;");

    // this->container = new QWidget(parent);

    this->wndTitle = new QLabel(tr("Change account name"), this);
    this->wndTitle->setStyleSheet("font-size: 16px; margin-bottom: 6px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    this->newUsername = new QLineEdit(this);
    this->newUsername->setPlaceholderText(tr("Enter new name"));
    this->newUsername->setStyleSheet("background-color: transparent; border: 1px solid #727272; font-size: 15px; width: 287px; height: 43px; margin-top: 10px;");

    ThemeManager::instance().loadTheme();

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: #7C7DD1");

    QColor color = ThemeManager::instance().buttonColor();
    this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->cancel->setStyleSheet("width: 153px; height: 48px; background-color: transparent; border: none; color: " + color.name());
    });

    connect(this->cancel, &QPushButton::clicked, this, &ChangeAccountNameWnd::close);

    this->edit = new QPushButton(tr("Edit"), this);
    this->edit->setStyleSheet("width: 153px; height: 48px; background-color: #8687E7; color: #fff");

    this->edit->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->edit->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: " + color.name());
    });

    connect(this->edit, &QPushButton::clicked, this, [this] {
        if (this->newUsername->text().isEmpty()) {
            WarningWnd* warning = new WarningWnd(tr("New username must not be empty!"), this);
            warning->showWithAnimation();
            return;
        }

        this->changeNameInDB();
    });

    this->btnsLayout = new QHBoxLayout();
    this->btnsLayout->addWidget(this->cancel);
    this->btnsLayout->addWidget(this->edit);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(this->wndTitle);
    this->mainLayout->addWidget(this->line);
    this->mainLayout->addWidget(this->newUsername);
    this->mainLayout->addStretch();
    this->mainLayout->addLayout(this->btnsLayout);

    this->setLayout(this->mainLayout);
}

ChangeAccountNameWnd::~ChangeAccountNameWnd() {}

void ChangeAccountNameWnd::changeNameInDB() {
    QSqlQuery query;

    query.prepare("SELECT id, name FROM users WHERE id = :id");
    query.bindValue(":id", UserSession::getUserId());

    if (query.exec()) {
        if (query.next()) {
            this->oldName = query.value("name").toString();

            qDebug() << "old name:" << query.value("name").toString();
            qDebug() << "new name: " << this->newUsername->text().trimmed();
        }
    } else
        qDebug() << "SQL error: " << query.lastError();

    if (this->newUsername->text().trimmed() != this->oldName.trimmed())
    {
        query.prepare("UPDATE users SET name = :name WHERE id = :id");

        query.bindValue(":id", UserSession::getUserId());
        query.bindValue(":name", this->newUsername->text());

        if (!query.exec()) {
            qDebug() << "Ошибка при обновлении задачи:" << query.lastError().text();
        }
    } else
    {
        WarningWnd* warning = new WarningWnd(tr("New name matches current name"), this);
        warning->showWithAnimation();
        return;
    }

    WarningWnd* warning = new WarningWnd(tr("New name is saved"), this);
    warning->showWithAnimation();

    emit this->onUpdateName();
    this->close();
}
