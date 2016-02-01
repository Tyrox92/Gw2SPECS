#ifndef SETTINGS_H
#define SETTINGS_H

#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QMainWindow>
#include <QSettings>



inline static QString ReadNameSettings(void);
inline static void WriteNameSettings(QString);

inline QString ReadNameSettings(void)
{
    QString tmp;
    QSettings settings("GW2DPS");
    settings.beginGroup("Name");
    tmp=settings.value("text").toString();
    settings.endGroup();
    return tmp;
}

inline void WriteNameSettings(QString text)
{

    QSettings settings("GW2DPS");
    settings.beginGroup("Name");
    settings.setValue("text",text);
    settings.endGroup();

}

namespace GW2
{



class Settings
    {
    public:
        template <typename WidgetType>
        inline static void ReadSettings(WidgetType* widget);

        template <typename WidgetType>
        inline static void WriteSettings(WidgetType* widget);



        static const QString s_Product;
        static const QString s_Version;

    private:
        Settings();
    };




    /*
    template <>
    inline void Settings::ReadSettings(QLineEdit* lineEdit)
    {
        QSettings settings(s_Product);

        settings.beginGroup(lineEdit->objectName());
        lineEdit->setText(settings.value("text"));
        settings.endGroup();

        ReadSettings<QWidget>(lineEdit);
    }

    template <>
    inline void Settings::WriteSettings(QLineEdit* lineEdit)
    {
        QSettings settings(s_Product);
        settings.beginGroup(lineEdit->objectName());
        settings.setValue("text",lineEdit->text());
        settings.endGroup();

        WriteSettings<QWidget>(lineEdit);
    }
*/


    template <>
    inline void Settings::ReadSettings(QWidget* widget)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(widget->objectName());
        widget->restoreGeometry(settings.value("geometry").toByteArray());
        settings.endGroup();
    }

    template <>
    inline void Settings::ReadSettings(QComboBox* comboBox)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(comboBox->objectName());
        comboBox->setCurrentIndex(settings.value("currentIndex").toInt());
        settings.endGroup();

        ReadSettings<QWidget>(comboBox);
    }

    template <>
    inline void Settings::ReadSettings(QMainWindow* mainWindow)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(mainWindow->objectName());
        mainWindow->restoreState(settings.value("windowState").toByteArray());
        settings.endGroup();

        ReadSettings<QWidget>(mainWindow);
    }

    template <>
    inline void Settings::WriteSettings(QWidget* widget)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(widget->objectName());
        settings.setValue("geometry", widget->saveGeometry());
        settings.endGroup();
    }

    template <>
    inline void Settings::WriteSettings(QComboBox* comboBox)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(comboBox->objectName());
        settings.setValue("currentIndex", comboBox->currentIndex());
        settings.endGroup();

        WriteSettings<QWidget>(comboBox);
    }

    template <>
    inline void Settings::WriteSettings(QMainWindow* mainWindow)
    {
        QSettings settings("GW2DPS");
        settings.beginGroup(mainWindow->objectName());
        settings.setValue("windowState", mainWindow->saveState());
        settings.endGroup();

        WriteSettings<QWidget>(mainWindow);
    }

    template <typename WidgetType>
    void Settings::ReadSettings(WidgetType* /*widget*/)
    {
        Q_ASSERT_X(false, "Settings::ReadSettings", "Unable to read settings. Class instance does not derive from QWidget.");
    }

    template <typename WidgetType>
    void Settings::WriteSettings(WidgetType* /*widget*/)
    {
        Q_ASSERT_X(false, "Settings::WriteSettings", "Unable to write settings. Class instance does not derive from QWidget.");
    }
}

#endif // SETTINGS_H
