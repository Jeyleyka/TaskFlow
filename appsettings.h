#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QApplication>

#include "profilesettingswidget.h"
#include "changelanguagewnd.h"
#include "themedialog.h"

class AppSettings : public QDialog
{
    Q_OBJECT
public:
    AppSettings(QWidget *parent = nullptr);
    ~AppSettings();

private:
    QHBoxLayout* headerLayout;

    QVBoxLayout* mainLayout;

    QPushButton* prevBtn;

    QLabel* wndTitle;

    ProfileSettingsWidget* changeAppColor;
    ProfileSettingsWidget* changeAppTypography;
    ProfileSettingsWidget* changeAppLanguage;
};

#endif // APPSETTINGS_H
