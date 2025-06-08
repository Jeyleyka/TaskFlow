#ifndef THEMEDIALOG_H
#define THEMEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QColor>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QHBoxLayout>

#include "thememanager.h"

class ThemeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThemeDialog(QWidget *parent = nullptr);
    ~ThemeDialog();

private:
    struct ColorItem {
        QPushButton* button;
        QLabel* preview;
    };

    QMap<QString, ColorItem> colorItems;
    QMap<QString, QColor> selectedColors;

    QVBoxLayout* mainLayout;

    void addColorOption(const QString& key, const QString& labelText, const QColor& defaultColor);
    void applyTheme();
};

#endif // THEMEDIALOG_H
