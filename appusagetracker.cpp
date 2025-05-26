#include "appusagetracker.h"
#include <QTimer>
#include <windows.h>
#include <psapi.h>

AppUsageTracker::AppUsageTracker(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &AppUsageTracker::updateUsage);
}

void AppUsageTracker::startTracking() {
    lastApp = getActiveProcessPath();
    lastSwitchTime = QDateTime::currentDateTime();
    timer.start(1000);
}

QString AppUsageTracker::getActiveProcessPath() {
    HWND hwnd = GetForegroundWindow();
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    TCHAR filename[MAX_PATH] = {0};

    if (hProcess) {
        if (GetModuleFileNameEx(hProcess, nullptr, filename, MAX_PATH)) {
            CloseHandle(hProcess);
            return QString::fromWCharArray(filename);
        }
        CloseHandle(hProcess);
    }
    return "Unknown";
}

void AppUsageTracker::updateUsage() {
    QString currentPath = getActiveProcessPath();
    QDateTime now = QDateTime::currentDateTime();
    qint64 elapsed = lastSwitchTime.msecsTo(now);

    if (!lastApp.isEmpty())
        todayUsage[lastApp].durationMs += elapsed;

    todayUsage[lastApp].exePath = lastApp;

    lastApp = currentPath;
    lastSwitchTime = now;
}

QMap<QString, AppUsageInfo> AppUsageTracker::getTodayUsage() const {
    return todayUsage;
}
