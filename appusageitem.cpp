#include "appusageitem.h"
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <windows.h>
#include <shellapi.h>

// Твоя функция из прошлого ответа
QPixmap getExeIcon(const QString& exePath, int size) {
    if (!QFileInfo::exists(exePath)) return QPixmap();

    HICON hIcon = nullptr;
    UINT count = ExtractIconExW(reinterpret_cast<LPCWSTR>(exePath.utf16()), 0, &hIcon, nullptr, 1);

    if (count > 0 && hIcon) {
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
        bi.biHeight = -size;
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

    return QPixmap();
}

AppUsageItem::AppUsageItem(const QString& appName, quint64 durationMs, const QString& exePath, QWidget* parent)
    : QWidget(parent)
{
    this->setFixedSize(500, 80);
    this->setStyleSheet("background-color: #363636; border-radius: 5px;");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);

    QPixmap icon = getExeIcon(exePath, 32);
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setFixedSize(32, 32);
    iconLabel->setPixmap(icon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QVBoxLayout* textLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(appName, this);
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: white;");

    QString formattedTime = formatTime(durationMs);
    QString details = QString("You spent %1 on %2 today").arg(formattedTime, appName);

    QLabel* timeLabel = new QLabel(details, this);
    timeLabel->setStyleSheet("color: #bbb; font-size: 12px;");

    textLayout->addWidget(nameLabel);
    textLayout->addWidget(timeLabel);

    mainLayout->addWidget(iconLabel);
    mainLayout->addLayout(textLayout);
    setLayout(mainLayout);
}

QString AppUsageItem::formatTime(qint64 ms) {
    int sec = ms / 1000;
    int min = sec / 60;
    int hrs = min / 60;
    min %= 60;
    return QString("%1h %2m").arg(hrs).arg(min, 2, 10, QLatin1Char('0'));
}
