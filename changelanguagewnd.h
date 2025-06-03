#ifndef CHANGELANGUAGEWND_H
#define CHANGELANGUAGEWND_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTranslator>
#include <QApplication>
#include <QSettings>
#include <QCoreApplication>
#include <QProcess>
#include <QFrame>

class ChangeLanguageWnd : public QDialog
{
    Q_OBJECT

public:
    ChangeLanguageWnd(QWidget *parent = nullptr);

signals:
    void changeLanguage();

private:
    QLabel* wndTitle;
    QLabel* warning;

    QFrame* line;

    QComboBox* languages;

    QPushButton* cancel;
    QPushButton* select;

    QHBoxLayout* btnsLayout;

    QVBoxLayout* mainLayout;

    QString lang;
};

#endif // CHANGELANGUAGEWND_H
