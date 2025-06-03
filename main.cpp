#include "mainwindow.h"
#include "login.h"
#include "intro.h"
#include "changelanguagewnd.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QTranslator>

void addToAutoStart() {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    settings.setValue("MyApp", appPath);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("MyCompany", "MyApp");
    QString lang = settings.value("language", "en_US").toString();

    QTranslator translator;

    if (lang != "English")
    {
        if (translator.load("translations/" + lang + ".qm")) {
            a.installTranslator(&translator);
        } else {
            qWarning() << "Не удалось загрузить перевод";
        }
    }

    a.setWindowIcon(QIcon(":/icons/logo.png"));
    addToAutoStart();
    Intro w;
    w.show();
    return a.exec();
}
