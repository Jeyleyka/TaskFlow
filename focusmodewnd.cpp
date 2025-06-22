#include "focusmodewnd.h"
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <windows.h>
#include <shellapi.h>

static QString formatTime(qint64 ms) {
    int sec = ms / 1000;
    int min = sec / 60;
    int hrs = min / 60;
    min %= 60;
    return QString("%1h %2m").arg(hrs).arg(min, 2, 10, QLatin1Char('0'));
}

QPixmap hiconToPixmap(HICON hIcon, int size) {
    if (!hIcon) return QPixmap();

    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    HDC hdc = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, size, size);
    HGDIOBJ old = SelectObject(hdcMem, hBitmap);

    DrawIconEx(hdcMem, 0, 0, hIcon, size, size, 0, nullptr, DI_NORMAL);

    BITMAPINFOHEADER bi;
    memset(&bi, 0, sizeof(BITMAPINFOHEADER));
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = size;
    bi.biHeight = -size; // отрицательная высота = top-down
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    QImage iconImg(size, size, QImage::Format_ARGB32);
    GetDIBits(hdcMem, hBitmap, 0, size, iconImg.bits(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    SelectObject(hdcMem, old);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdc);

    DestroyIcon(hIcon);

    return QPixmap::fromImage(iconImg);
}

FocusModeWnd::FocusModeWnd(QWidget* parent)
    : QWidget(parent), focusRunning(false)
{
    this->wndTitle = new QLabel(tr("Focus Mode"), this);
    this->wndTitle->setStyleSheet("font-size: 19px; color #fff; margin-bottom: 20px;");

    this->timerWidget = new CircularTimerWidget(this);
    this->timerWidget->setMinimumSize(280, 280);

    this->startStopFocusBtn = new QPushButton(tr("Start Focusing"), this);
    this->startStopFocusBtn->setStyleSheet("width: 200px; height: 40px; background-color: #8687E7; margin-top: 20px;");

    ThemeManager::instance().loadTheme();

    // Обновить после загрузки вручную:
    QColor color = ThemeManager::instance().buttonColor();
    this->startStopFocusBtn->setStyleSheet("width: 200px; height: 40px; margin-top: 20px; background-color: " + color.name());

    // Подключение на будущее
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().buttonColor();
        this->startStopFocusBtn->setStyleSheet("width: 200px; height: 40px; margin-top: 20px; background-color: " + color.name());
    });

    connect(&ThemeManager::instance(), &ThemeManager::btnChanged, this, [this](const QColor& color) {
        QString style = QString("width: 200px; height: 40px; background-color: #8687E7; margin-top: 20px; background-color: %1").arg(color.name());
        this->startStopFocusBtn->setStyleSheet(style);
    });

    connect(this->startStopFocusBtn, &QPushButton::clicked, this, &FocusModeWnd::startStopFocus);

    this->appList = new QListWidget(this);
    this->appList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->appList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->appList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QColor bgColor = ThemeManager::instance().backgroundColor();
    this->appList->setStyleSheet("background-color: " + bgColor.name());

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this] {
        QColor color = ThemeManager::instance().backgroundColor();
        this->appList->setStyleSheet("background-color: " + color.name());
    });

    connect(&ThemeManager::instance(), &ThemeManager::bgChanged, this, [this](const QColor& color) {
        QString style = QString("background-color: %1").arg(color.name());
        this->appList->setStyleSheet(style);
    });

    setStyleSheet(R"(
        QListWidget { background-color: #1e1e1e; border: none; }
        QListWidget::item { padding: 10px; }
    )");

    this->tracker = new AppUsageTracker(this);

    tracker->startTracking();

    uiUpdateTimer = new QTimer(this);
    connect(uiUpdateTimer, &QTimer::timeout, this, &FocusModeWnd::updateAppUsageList);
    uiUpdateTimer->start(5000); // обновлять каждые 5 сек

    this->applications = new QLabel(tr("Applications"), this);
    this->applications->setStyleSheet("font-size: 17px; color #fff; margin-top: 10px;");

    this->navBar = new NavigationBar(this);

    connect(this->navBar, &NavigationBar::onShowTaskDialog, this, [this] {
        this->showTaskDialog();
    });

    connect(this->navBar, &NavigationBar::switchToIndex, this, &FocusModeWnd::switchToIndex);
    connect(this->navBar, &NavigationBar::switchToCalendar, this, &FocusModeWnd::switchToCalendar);
    connect(this->navBar, &NavigationBar::switchToProfile, this, &FocusModeWnd::switchToProfile);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setContentsMargins(0,0,0,0);
    this->mainLayout->addWidget(this->wndTitle, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->timerWidget, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->startStopFocusBtn, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->applications, 0);
    this->mainLayout->addWidget(this->appList);
    this->mainLayout->addStretch();
    this->mainLayout->addWidget(this->navBar);

    this->setLayout(this->mainLayout);
}

void FocusModeWnd::showTaskDialog() {
    if (this->addTaskWnd && this->addTaskWnd->isVisible()) {
        this->addTaskWnd->raise();
        this->addTaskWnd->activateWindow();
        return;
    }

    this->addTaskWnd = new AddTask(this);

    this->addTaskWnd->show();

    // connect(this->addTaskWnd, &TaskDialog::accepted, this, [this]() {
    //     Task task = this->dialog->getTask();
    //     if (this->dataBase->insertTaskToDatabase(task)) {
    //         QString formattedDate = task.formatDateTime(task.dueDate);

    //         auto* taskUI = new TaskUI(task.title, task.description, formattedDate, task.priority, task.categoryName, task.categoryColor, task.categoryIcon, task.id, task.completed, this);
    //         taskUI->setFixedSize(920, 100);
    //         this->tasks.append(taskUI);
    //         this->tasksLayout->addWidget(taskUI, 0, Qt::AlignHCenter);

    //         connect(taskUI, &TaskUI::taskClicked, this, [=] {
    //             TaskInfo* taskInfo = new TaskInfo(task.id, task.title, task.description, task.dueDate, taskUI, this);
    //             taskInfo->setFixedSize(500, 600);
    //             taskInfo->show();

    //             connect(taskInfo, &TaskInfo::onChangeUI, this, [this, taskUI, taskInfo] {
    //                 qDebug() << "Signal onChangeUI received";

    //                 if (!taskUI || !taskInfo) return;

    //                 QString title = taskInfo->getTitle();
    //                 QString desc = taskInfo->getDesc();

    //                 qDebug() << "Got data:" << title << desc;

    //                 taskUI->setTitle(title);
    //                 taskUI->setCategory(taskInfo->getCategoryName(), taskInfo->getCategoryColor(), taskInfo->getCategoryIcon(), 14,14);
    //                 // taskUI->setDesc(desc);
    //             });
    //         });
    //         // this->model->addTask(task);
    //     }

    //     this->addTaskWnd->deleteLater();
    //     this->addTaskWnd = nullptr;
    // });

    // connect(this->addTaskWnd, &TaskDialog::rejected, this, [this]() {
    //     this->addTaskWnd->deleteLater();
    //     this->addTaskWnd = nullptr;
    // });
}

void FocusModeWnd::startStopFocus() {
    if (this->focusRunning) {
        this->timerWidget->stop();
        this->startStopFocusBtn->setText(tr("Start Focus"));
    } else
    {
        this->timerWidget->start();
        this->startStopFocusBtn->setText(tr("Stop Focus"));
    }

    this->focusRunning = !this->focusRunning;
}

void FocusModeWnd::updateAppUsageList() {
    appList->clear();
    this->appList->setFixedWidth(540); // чуть шире AppUsageItem

    auto usage = tracker->getTodayUsage();

    for (auto it = usage.begin(); it != usage.end(); ++it) {
        QString exePath = it.value().exePath;
        QString appName = QFileInfo(exePath).baseName();

        if (QFileInfo(exePath).fileName().compare("todo_app.exe", Qt::CaseInsensitive) == 0)
            continue;

        QListWidgetItem *item = new QListWidgetItem(appList);
        item->setSizeHint(QSize(540, 90)); // чтобы вместить контейнер

        // Твой AppUsageItem
        AppUsageItem* usageWidget = new AppUsageItem(appName, it.value().durationMs, exePath);

        // Оборачиваем в контейнер и центрируем
        QWidget* wrapper = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(wrapper);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(usageWidget);

        appList->addItem(item);
        appList->setItemWidget(item, wrapper);
    }
}


