#ifndef PROFILESETTINGSWIDGET_H
#define PROFILESETTINGSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

#include "thememanager.h"

class ProfileSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    ProfileSettingsWidget(const QIcon& icon, const QString& str, QWidget* parent = nullptr);

signals:
    void onClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QWidget* container;

    QHBoxLayout* mainLayout;

    QLabel* text;

    QPushButton* iconBtn;
    QPushButton* arrow;
};

#endif // PROFILESETTINGSWIDGET_H
