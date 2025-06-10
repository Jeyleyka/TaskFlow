#include "profilewnd.h"

ProfileWnd::ProfileWnd(QWidget* parent)
    : QWidget(parent)
{
    this->mainLayout = new QVBoxLayout(this);
    this->setLayout(this->mainLayout);
    this->mainLayout->setContentsMargins(0, 20, 0, 0);
    this->mainLayout->setSpacing(0);

    this->wndTitle = new QLabel(tr("Profile"), this);
    this->wndTitle->setStyleSheet("font-size: 19px; color: #fff; margin-bottom: 10px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    QSqlQuery query;
    query.prepare("SELECT icon, name FROM users WHERE id = :id");
    query.bindValue(":id", UserSession::getUserId());

    if (query.exec() && query.next()) {
        QByteArray imageData = query.value("icon").toByteArray();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        QString usernameStr = query.value("name").toString();

        this->iconLabel = new QLabel();

        this->iconLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this->iconLabel->setFixedSize(64, 64);
        this->iconLabel->setStyleSheet("width: 64px; height: 64px; margin-right: 480px; border: none");

        QHBoxLayout* iconLayout = new QHBoxLayout;
        iconLayout->addStretch();
        iconLayout->addWidget(iconLabel);
        iconLayout->addStretch();

        QLabel* username = new QLabel(usernameStr, this);
        username->setStyleSheet("font-size: 19px; color: #fff; margin-top: 10px;");
        username->setAlignment(Qt::AlignHCenter);

        this->mainLayout->addWidget(this->wndTitle);
        this->mainLayout->addLayout(iconLayout);
        this->mainLayout->addWidget(username);
    }

    this->initTasksStatus();

    this->settings = new QLabel(tr("Settings"), this);
    this->settings->setStyleSheet("font-size: 14px; color: #888888; margin-top: 10px;");
    this->settings->setAlignment(Qt::AlignHCenter);

    this->account = new QLabel(tr("Account"), this);
    this->account->setStyleSheet("font-size: 14px; color: #888888; margin-top: 10px;");
    this->account->setAlignment(Qt::AlignHCenter);

    this->taskFlow = new QLabel(tr("TaskFlow"), this);
    this->taskFlow->setStyleSheet("font-size: 14px; color: #888888; margin-top: 10px;");
    this->taskFlow->setAlignment(Qt::AlignHCenter);

    this->changeAppColor = new ProfileSettingsWidget(QIcon(":/icons/brush.png"), tr("Change app color"), this);

    connect(this->changeAppColor, &ProfileSettingsWidget::onClicked, this, [this] {
        ThemeDialog* themeDialog = new ThemeDialog(this);
        themeDialog->show();
    });

    this->changeAppLanguage = new ProfileSettingsWidget(QIcon(":/icons/language.png"), tr("Change app language"), this);

    connect(this->changeAppLanguage, &ProfileSettingsWidget::onClicked, this, [this] {
        ChangeLanguageWnd* languageWnd = new ChangeLanguageWnd(this);
        languageWnd->show();
    });

    this->changeAccNameBtn = new ProfileSettingsWidget(QIcon(":/icons/user.png"), tr("Change account name"), this);

    connect(this->changeAccNameBtn, &ProfileSettingsWidget::onClicked, this, [this] {
        ChangeAccountNameWnd* changeWnd = new ChangeAccountNameWnd(this);
        changeWnd->show();

        connect(changeWnd, &ChangeAccountNameWnd::onUpdateName, this, [this] {
            QSqlQuery query;

            if (query.exec("SELECT name FROM user LIMIT 1")) {
                while (query.next()) {
                    QString name = query.value("name").toString();

                    this->username->setText(name);
                }
            }
        });
    });

    this->changePasswBtn = new ProfileSettingsWidget(QIcon(":/icons/key.png"), tr("Change account password"), this);

    connect(this->changePasswBtn, &ProfileSettingsWidget::onClicked, this, [this] {
        ChangePass* changePassWnd = new ChangePass(this);
        changePassWnd->show();
    });

    this->changeAccImg = new ProfileSettingsWidget(QIcon(":/icons/camera.png"), tr("Change account image"), this);

    connect(this->changeAccImg, &ProfileSettingsWidget::onClicked, this, [this] {
        this->changeImgWnd = new ChangeImgWnd(this);
        this->changeImgWnd->show();

        connect(this->changeImgWnd, &ChangeImgWnd::onUpdateImg, this, [this] {
            QSqlQuery query;
            query.prepare("SELECT icon FROM users WHERE id = :id");
            query.bindValue(":id", UserSession::getUserId());

            qDebug() << "current id: " << UserSession::getUserId();

            if (query.exec()) {
                while (query.next()) {
                    QByteArray imageData = query.value("icon").toByteArray();
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);

                    this->iconLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    this->iconLabel->setStyleSheet("width: 64px; height: 64px; margin-right: 480px; border: none");
                    this->iconLabel->setFixedSize(64, 64);

                    emit this->onChangeImg(pixmap);
                }
            }
        });
    });

    this->aboutUsBtn = new ProfileSettingsWidget(QIcon(":/icons/category.png"), tr("About the developer"), this);

    connect(this->aboutUsBtn, &ProfileSettingsWidget::onClicked, this, [this] {
        this->about = new AboutUsWnd(this);
        this->about->show();
    });

    this->helpAndFeedBtn = new ProfileSettingsWidget(QIcon(":/icons/lightning-bolt.png"), tr("Help & Feedback"), this);

    connect(this->helpAndFeedBtn, &ProfileSettingsWidget::onClicked, this, [this] {
        this->feedback = new HelpAndFeedbackWnd(this);
        this->feedback->show();
    });

    QVBoxLayout* contentLayout = new QVBoxLayout;
    contentLayout->setSpacing(10);
    contentLayout->addWidget(this->settings);
    contentLayout->addWidget(this->changeAppColor);
    contentLayout->addWidget(this->changeAppLanguage);
    contentLayout->addWidget(this->account);
    contentLayout->addWidget(this->changeAccNameBtn);
    contentLayout->addWidget(this->changePasswBtn);
    contentLayout->addWidget(this->changeAccImg);
    contentLayout->addWidget(this->taskFlow);
    contentLayout->addWidget(this->aboutUsBtn);
    contentLayout->addWidget(this->helpAndFeedBtn);

    QWidget* container = new QWidget;
    container->setLayout(contentLayout);

    QVBoxLayout* centerLayout = new QVBoxLayout(this);
    centerLayout->addStretch();
    centerLayout->addWidget(container, 0, Qt::AlignHCenter); // по центру по горизонтали
    centerLayout->addStretch();

    this->logOutBtn = new QPushButton(this);
    this->logOutBtn->setIcon(QIcon(":/icons/log-out.png"));
    this->logOutBtn->setIconSize(QSize(24,24));
    this->logOutBtn->setText(tr("Log out"));
    this->logOutBtn->setStyleSheet("background-color: transparent; border: none; color: #FF3838");

    connect(this->logOutBtn, &QPushButton::clicked, this, [this] {
        emit this->onLogOut();
    });

    this->mainLayout->addLayout(centerLayout);
    this->mainLayout->addWidget(this->logOutBtn);

    this->navBar = new NavigationBar(this);

    // connect(this->navBar, &NavigationBar::onShowTaskDialog, this, [this] {
    //     this->showTaskDialog();
    // });

    connect(this->navBar, &NavigationBar::switchToIndex, this, &ProfileWnd::switchToIndex);
    connect(this->navBar, &NavigationBar::switchToCalendar, this, &ProfileWnd::switchToCalendar);
    connect(this->navBar, &NavigationBar::switchToFocus, this, &ProfileWnd::switchToFocus);

    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->navBar);
}

ProfileWnd::~ProfileWnd() {}

void ProfileWnd::initTasksStatus() {
    QSqlQuery query;

    int completedCount = 0;

    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id = :user_id AND completed = 1");
    query.bindValue(":user_id", UserSession::getUserId());
    if (query.exec() && query.next()) {
        completedCount = query.value(0).toInt();
    }

    qDebug() << "COMPLETED TASKS: " << completedCount;

    // === Загружаем общее количество задач ===
    int totalCount = 0;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id = :user_id");
    query.bindValue(":user_id", UserSession::getUserId());
    if (query.exec() && query.next()) {
        totalCount = query.value(0).toInt();
    }

    this->tasksDone = new QLabel(QString::number(completedCount) + tr(" Task done"), this);
    this->tasksDone->setStyleSheet("min-width: 160px; min-height: 50px; background-color: #363636; border-radius: 5px;");
    this->tasksDone->setAlignment(Qt::AlignCenter);

    ThemeManager::instance().loadTheme();

    QColor color = ThemeManager::instance().widgetsColor();
    this->tasksDone->setStyleSheet("min-width: 160px; min-height: 50px; border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().widgetsColor();
        this->tasksDone->setStyleSheet("min-width: 160px; min-height: 50px; border-radius: 5px; background-color: " + color.name());
    });

    this->tasksLeft = new QLabel(QString::number(totalCount - completedCount) + tr(" Task left"), this);
    this->tasksLeft->setStyleSheet("min-width: 160px; min-height: 50px; background-color: #363636; border-radius: 5px;");
    this->tasksLeft->setAlignment(Qt::AlignCenter);

    this->tasksLeft->setStyleSheet("min-width: 160px; min-height: 50px; border-radius: 5px; background-color: " + color.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().widgetsColor();
        this->tasksLeft->setStyleSheet("min-width: 160px; min-height: 50px; border-radius: 5px; background-color: " + color.name());
    });

    QHBoxLayout* tasksLayout = new QHBoxLayout;
    tasksLayout->addStretch();
    tasksLayout->addWidget(tasksLeft);
    tasksLayout->addSpacing(13);
    tasksLayout->addWidget(tasksDone);
    tasksLayout->addStretch();
    tasksLayout->setContentsMargins(0, 20, 0, 0);

    this->mainLayout->addLayout(tasksLayout);
}

void ProfileWnd::updateTasksData() {
    qDebug() << "updateTasksData вызвана";
    QSqlQuery query;

    int completedCount = 0;

    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id = :user_id AND completed = 1");
    query.bindValue(":user_id", UserSession::getUserId());
    if (query.exec() && query.next()) {
        completedCount = query.value(0).toInt();
    }

    qDebug() << "COMPLETED TASKS: " << completedCount;

    int totalCount = 0;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id = :user_id");
    query.bindValue(":user_id", UserSession::getUserId());
    if (query.exec() && query.next()) {
        totalCount = query.value(0).toInt();
    }

    this->tasksDone->setText(QString::number(completedCount) + tr(" Task done"));
    this->tasksLeft->setText(QString::number(totalCount - completedCount) + tr(" Task left"));
}

// void ProfileWnd::showTaskDialog() {
//     if (this->dialog && this->dialog->isVisible()) {
//         this->dialog->raise();
//         this->dialog->activateWindow();
//         return;
//     }

//     this->dialog = new TaskDialog(this);
//     this->dialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
//     this->dialog->setModal(true);

//     int dialogHeight = 400;
//     int dialogWidth = this->width();

//     QPoint startPos(this->x(), this->y() + this->height());
//     QPoint endPos(this->x(), this->y() + this->height() - dialogHeight);

//     dialog->setGeometry(startPos.x(), startPos.y(), dialogWidth, dialogHeight);
//     dialog->show();

//     QPropertyAnimation* show = new QPropertyAnimation(this->dialog, "pos");
//     show->setDuration(600);
//     show->setStartValue(startPos);
//     show->setEndValue(endPos);
//     show->setEasingCurve(QEasingCurve::OutCubic);
//     show->start(QAbstractAnimation::DeleteWhenStopped);

//     connect(this->dialog, &TaskDialog::accepted, this, [this]() {
//         Task task = this->dialog->getTask();
//         if (this->dataBase->insertTaskToDatabase(task)) {
//             QString formattedDate = task.formatDateTime(task.dueDate);

//             auto* taskUI = new TaskUI(task.title, task.description, formattedDate, task.priority, task.categoryName, task.categoryColor, task.categoryIcon, task.id, task.completed, this);
//             taskUI->setFixedSize(920, 100);
//             this->tasks.append(taskUI);
//             this->tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

//             connect(taskUI, &TaskUI::onUpdateTaskToComplete, this, [this, taskUI] {
//                 this->tasks.removeOne(taskUI);
//                 this->completedTasks.append(taskUI);
//                 tasksLayout->removeWidget(taskUI);
//                 this->completeTaskslayout->addWidget(taskUI, 1, Qt::AlignHCenter);
//             });

//             connect(taskUI, &TaskUI::onUpdateTaskToNotComplete, this, [this, taskUI] {
//                 this->completedTasks.removeOne(taskUI);
//                 this->tasks.append(taskUI);
//                 this->completeTaskslayout->removeWidget(taskUI);
//                 tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);
//             });

//             connect(taskUI, &TaskUI::taskClicked, this, [=] {
//                 TaskInfo* taskInfo = new TaskInfo(task.id, task.title, task.description, task.formatDateTime(task.dueDate), taskUI, this);
//                 taskInfo->setFixedSize(500, 600);
//                 taskInfo->show();

//                 connect(taskInfo, &TaskInfo::onChangeUI, this, [this, taskUI, taskInfo] {
//                     qDebug() << "Signal onChangeUI received";

//                     if (!taskUI || !taskInfo) return;

//                     QString title = taskInfo->getTitle();
//                     QString desc = taskInfo->getDesc();

//                     qDebug() << "Got data:" << title << desc;

//                     taskUI->setTitle(title);
//                     taskUI->setCategory(taskInfo->getCategoryName(), taskInfo->getCategoryColor(), taskInfo->getCategoryIcon(), 14,14);
//                     // taskUI->setDesc(desc);
//                 });
//             });
//             // this->model->addTask(task);
//         }

//         this->dialog->deleteLater();
//         this->dialog = nullptr;
//     });

//     connect(this->dialog, &TaskDialog::rejected, this, [this]() {
//         this->dialog->deleteLater();
//         this->dialog = nullptr;
//     });
// }
