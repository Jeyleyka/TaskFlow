#include "mainwindow.h"
#include "login.h"
#include "intro.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>

void addToAutoStart() {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    settings.setValue("MyApp", appPath);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    addToAutoStart();
    Intro w;
    w.show();
    return a.exec();
}
