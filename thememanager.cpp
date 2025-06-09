#include "thememanager.h"

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

QColor ThemeManager::backgroundColor() const {
    return this->m_backgroundColor;
}

void ThemeManager::setBackgroundColor(const QColor &color) {
    if (color != this->m_backgroundColor) {
        this->m_backgroundColor = color;
        emit this->bgChanged(color);
        emit themeChanged();
    }
}

QColor ThemeManager::widgetsColor() const {
    return this->m_widgetsColor;
}

void ThemeManager::setWidgetsColor(const QColor &color) {
    if (color != this->m_widgetsColor) {
        this->m_widgetsColor = color;
        emit this->widgetsChanged(color);
        emit themeChanged();
    }
}

QColor ThemeManager::navbarColor() const {
    return this->m_navBarColor;
}

void ThemeManager::setNavbarColor(const QColor &color) {
    if (color != this->m_navBarColor) {
        this->m_navBarColor = color;
        emit this->navbarChanged(color);
        emit themeChanged();
    }
}

QColor ThemeManager::buttonColor() const {
    return this->m_customActiveButtonColorSet ? this->m_buttonColor : QColor("#8182DE");
}

void ThemeManager::setButtonColor(const QColor &color) {
    if (color != this->m_buttonColor) {
        this->m_buttonColor = color;
        this->m_customActiveButtonColorSet = true;
        emit this->btnChanged(color);
        emit themeChanged();
    }
}

void ThemeManager::saveTheme() const {
    QSettings settings("Company", "TaskFlow");
    settings.setValue("theme/background", m_backgroundColor);
    settings.setValue("theme/widgets", m_widgetsColor);
    settings.setValue("theme/navbar", m_navBarColor);
    settings.setValue("theme/button", m_buttonColor);

    qDebug() << "color: " << m_buttonColor;
}

void ThemeManager::loadTheme() {
    QSettings settings("Company", "TaskFlow"); // те же значения

    this->setBackgroundColor(settings.value("theme/background", QColor("#121212")).value<QColor>());
    this->setWidgetsColor(settings.value("theme/widgets",    QColor("#1E1E1E")).value<QColor>());
    this->setNavbarColor(settings.value("theme/navbar",     QColor("#2D2D2D")).value<QColor>());
    this->setButtonColor(settings.value("theme/button",     QColor("#3A3A3A")).value<QColor>());
}


void ThemeManager::resetToDefault() {
    m_backgroundColor = QColor("#1E1E1E");
    m_widgetsColor = QColor("#363636");
    m_navBarColor = QColor("#363636");
    m_buttonColor = QColor("#8687E7");

    emit this->themeChanged();
}



