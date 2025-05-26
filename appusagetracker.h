#pragma once

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QTimer>

struct AppUsageInfo {
    QString exePath;
    qint64 durationMs = 0;
};

class AppUsageTracker : public QObject {
    Q_OBJECT
public:
    explicit AppUsageTracker(QObject *parent = nullptr);
    void startTracking();
    QMap<QString, AppUsageInfo> getTodayUsage() const;

private:
    QTimer timer;
    QString lastApp;
    QDateTime lastSwitchTime;
    QMap<QString, AppUsageInfo> todayUsage;

    QString getActiveProcessPath();
    void updateUsage();
};
