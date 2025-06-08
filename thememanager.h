#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QSettings>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager& instance();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    QColor widgetsColor() const;
    void setWidgetsColor(const QColor& color);

    QColor navbarColor() const;
    void setNavbarColor(const QColor& color);

    QColor buttonColor() const;
    void setButtonColor(const QColor& color);

    void saveTheme() const;
    void loadTheme();
    void resetToDefault();

signals:
    void bgChanged(const QColor& color);
    void widgetsChanged(const QColor& color);
    void navbarChanged(const QColor& color);
    void btnChanged(const QColor& color);
    void themeChanged();

private:
    ThemeManager() = default;
    QColor m_backgroundColor;
    QColor m_widgetsColor;
    QColor m_navBarColor;
    QColor m_buttonColor;

    bool m_customActiveButtonColorSet = false;
};

#endif // THEMEMANAGER_H
