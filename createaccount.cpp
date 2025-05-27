#include "createaccount.h"

CreateAccount::CreateAccount(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tasks.db");

    QWidget* container = new QWidget(this);

    this->prevWndBtn = new QPushButton("<", this);
    this->prevWndBtn->setStyleSheet("background-color: transparent; border: none; font-size: 30px;");

    connect(this->prevWndBtn, &QPushButton::clicked, this, [this] {
        Intro* intro = new Intro();
        intro->show();
        this->close();
    });

    this->closeWndBtn = new QPushButton("×", this);
    this->closeWndBtn->setStyleSheet("background-color: transparent; border: none; font-size: 31px;");

    connect(this->closeWndBtn, &QPushButton::clicked, this, &CreateAccount::close);

    this->btnsLayout = new QHBoxLayout();
    this->btnsLayout->addWidget(this->prevWndBtn);
    this->btnsLayout->addStretch(1);
    this->btnsLayout->addWidget(this->closeWndBtn);

    this->wndTitle = new QLabel("Register", this);
    this->wndTitle->setStyleSheet("font-size: 20px; font-weight: 600; color: #fff; margin-top: 20px;");

    QVBoxLayout* titleLayout = new QVBoxLayout;
    titleLayout->setContentsMargins(0, 20, 0, 20);  // Верхний и нижний отступ
    titleLayout->addWidget(this->wndTitle);

    this->usernameLabel = new QLabel("Username", this);
    this->usernameLabel->setStyleSheet("font-size: 15px; color: #fff");

    this->usernameEdit = new QLineEdit(this);
    this->usernameEdit->setPlaceholderText("Enter your Username");
    this->usernameEdit->setStyleSheet("width: 350px; height: 50px; background-color: #363636; border-radius: 5px;");

    this->usernameLayout = new QVBoxLayout;
    this->usernameLayout->setContentsMargins(0, 5, 0, 5);
    this->usernameLayout->setSpacing(0);
    this->usernameLayout->addWidget(this->usernameLabel);
    this->usernameLayout->addWidget(this->usernameEdit);

    this->passwordLabel = new QLabel("Password", this);
    this->passwordLabel->setStyleSheet("font-size: 15px; color: #fff");

    this->passwordEdit = new QLineEdit(this);
    this->passwordEdit->setPlaceholderText("Enter your Password");
    this->passwordEdit->setEchoMode(QLineEdit::Password);
    this->passwordEdit->setStyleSheet("width: 350px; height: 50px; background-color: #363636; border-radius: 5px;");

    this->passwordLayout = new QVBoxLayout;
    this->passwordLayout->setContentsMargins(0, 5, 0, 5);
    this->passwordLayout->setSpacing(0);
    this->passwordLayout->addWidget(this->passwordLabel);
    this->passwordLayout->addWidget(this->passwordEdit);

    this->confirmPassLabel = new QLabel("Confirm Password", this);
    this->confirmPassLabel->setStyleSheet("font-size: 15px; color: #fff");

    this->confirmPassEdit = new QLineEdit(this);
    this->confirmPassEdit->setPlaceholderText("Confirm Password");
    this->confirmPassEdit->setEchoMode(QLineEdit::Password);
    this->confirmPassEdit->setStyleSheet("width: 350px; height: 50px; background-color: #363636; border-radius: 5px;");

    this->confirmPassLayout = new QVBoxLayout;
    this->confirmPassLayout->setContentsMargins(0, 5, 0, 5);
    this->confirmPassLayout->setSpacing(0);
    this->confirmPassLayout->addWidget(this->confirmPassLabel);
    this->confirmPassLayout->addWidget(this->confirmPassEdit);

    this->loginBtn = new QPushButton("Register", this);
    this->loginBtn->setStyleSheet("width: 350px; height: 50px; background-color: #8687E7; border-radius: 5px; margin-top: 20px;");

    connect(this->loginBtn, &QPushButton::clicked, this, [this] {
        if (this->usernameEdit->text().isEmpty() || this->passwordEdit->text().isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Area must not be empty!");
            return;
        }

        if (this->confirmPassEdit->text() != this->passwordEdit->text())
        {
            QMessageBox::warning(this, "Warning", "Passwords do not match!");
            return;
        }

        if (this->createAccToDB(this->usernameEdit->text(), this->passwordEdit->text())) {
            MainWindow* mainWnd = new MainWindow();
            mainWnd->show();
            this->close();
        }
    });

    this->haventAccount = new QPushButton("Already have an account? Login", this);
    this->haventAccount->setStyleSheet("background-color: transparent; border: none; font-size: 15px; margin-top: 20px;");

    connect(this->haventAccount, &QPushButton::clicked, this, [this] {
        Login* login = new Login();
        login->show();
        this->close();
    });

    this->mainLayout = new QVBoxLayout(container);
    this->mainLayout->addLayout(this->btnsLayout);
    this->mainLayout->addLayout(titleLayout);
    this->mainLayout->addLayout(this->usernameLayout);
    this->mainLayout->addLayout(this->passwordLayout);
    this->mainLayout->addLayout(this->confirmPassLayout);
    this->mainLayout->addWidget(this->loginBtn, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->haventAccount, 0, Qt::AlignHCenter);
    this->mainLayout->setContentsMargins(10, 10, 10, 10);
    // this->mainLayout->setSpacing(10);
    // this->setLayout(this->mainLayout);

    this->setFixedSize(375, 550);
}

CreateAccount::~CreateAccount() {}

bool CreateAccount::createAccToDB(const QString& name, const QString& password) {
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qDebug() << "База данных не открыта!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO user (name, password, icon) VALUES (?,?,?)");

    QByteArray iconBytes;
    QPixmap pixmap = QIcon(":/icons/profile-picture.png").pixmap(64, 64);
    QBuffer buffer(&iconBytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    query.addBindValue(name);
    query.addBindValue(password);
    query.addBindValue(iconBytes);

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Пользователь добавлен успешно!";
        return true;
    }

    return false;
}
