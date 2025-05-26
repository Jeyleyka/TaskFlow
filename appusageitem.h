#ifndef APPUSAGEITEM_H
#define APPUSAGEITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <windows.h>
#include <shellapi.h>

QPixmap getExeIcon(const QString& exePath, int size = 32);

class AppUsageItem : public QWidget
{
public:
    AppUsageItem(const QString &appName, quint64 durationMs, const QString& exePath, QWidget *parent = nullptr);

private:
    static QString formatTime(qint64 ms);
};

#endif // APPUSAGEITEM_H
