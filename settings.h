#ifndef SETTINGS_H
#define SETTINGS_H

#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QMainWindow>
#include <QSettings>
#include <QCheckBox>
#include <QDebug>


inline static QString ReadNameSettings(void);
inline static void WriteNameSettings(QString);
inline static QString ReadIPSettings(void);
inline static void WriteIPSettings(QString);

inline static QString ReadPortSettings(void);
inline static void WritePortSettings(QString);

inline static QString Read1stRun(void);
inline static void Write1stRun(QString);

inline static QString ReadToolbarSettings(void);
inline static void WriteToolbarSettings(QString);

inline static QString ReadGraphSettings(void);
inline static void WriteGraphSettings(QString);


inline QString Read1stRun(void)
{
    QString tmp;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("FirstRun");
    tmp=settings.value("text").toString();
    settings.endGroup();
    return tmp;
}

inline void Write1stRun(QString text)
{

    QSettings settings("Gw2SPECS");
    settings.beginGroup("FirstRun");
    settings.setValue("text",text);
    settings.endGroup();

}

inline QString ReadNameSettings(void)
{
    QString tmp;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("Name");
    tmp=settings.value("text").toString();
    settings.endGroup();
    return tmp;
}
inline void WriteNameSettings(QString text)
{

    QSettings settings("Gw2SPECS");
    settings.beginGroup("Name");
    settings.setValue("text",text);
    settings.endGroup();

}

inline QString ReadIPSettings(void)
{
    QString tmpip;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("IP");
    tmpip=settings.value("text").toString();
    settings.endGroup();
    return tmpip;
}
inline void WriteIPSettings(QString text)
{

    QSettings settings("Gw2SPECS");
    settings.beginGroup("IP");
    settings.setValue("text",text);
    settings.endGroup();

}

inline QString ReadPortSettings(void)
{
    QString tmpPort;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("Port");
    tmpPort=settings.value("text").toString();
    settings.endGroup();
    return tmpPort;
}
inline void WritePortSettings(QString text)
{

    QSettings settings("Gw2SPECS");
    settings.beginGroup("Port");
    settings.setValue("text",text);
    settings.endGroup();

}

inline QString ReadToolbarSettings(void)
{
    QString tmpToolbar;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("toolBarHidden");
    tmpToolbar=settings.value("text").toString();
    settings.endGroup();
    return tmpToolbar;
}
inline void WriteToolbarSettings(QString text)
{

    QSettings settings("Gw2SPECS");
    settings.beginGroup("toolBarHidden");
    settings.setValue("text",text);
    settings.endGroup();

}


inline QString ReadGraphSettings(void)
{
    QString tmpGraph;
    QSettings settings("Gw2SPECS");
    settings.beginGroup("graphHidden");
    tmpGraph=settings.value("text").toString();
    settings.endGroup();
    return tmpGraph;
}
inline void WriteGraphSettings(QString text)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup("graphHidden");
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
    inline void Settings::ReadSettings(QLineEdit* InputName)
    {
        QSettings settings(s_Product);

        settings.beginGroup(InputName->objectName());
        InputName->setText(settings.value("text"));
        settings.endGroup();

        ReadSettings<QWidget>(InputName);
    }

    template <>
    inline void Settings::WriteSettings(QLineEdit* InputName)
    {
        QSettings settings(s_Product);
        settings.beginGroup(InputName->objectName());
        settings.setValue("text",InputName->text());
        settings.endGroup();

        WriteSettings<QWidget>(InputName);
    }
*/


/**************************************************
*                                                 *
*               Template Widget                   *
*                                                 *
***************************************************/

template <>
inline void Settings::ReadSettings(QWidget* widget)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(widget->objectName());
    widget->restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
}

template <>
inline void Settings::WriteSettings(QWidget* widget)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(widget->objectName());
    settings.setValue("geometry", widget->saveGeometry());
    settings.endGroup();
}

/**************************************************
*                                                 *
*               Template Combobox                 *
*                                                 *
***************************************************/

template <>
inline void Settings::ReadSettings(QComboBox* comboBox)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(comboBox->objectName());
    comboBox->setCurrentIndex(settings.value("currentIndex").toInt());
    settings.endGroup();

    ReadSettings<QWidget>(comboBox);
}

template <>
inline void Settings::WriteSettings(QComboBox* comboBox)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(comboBox->objectName());
    settings.setValue("currentIndex", comboBox->currentIndex());
    settings.endGroup();

    WriteSettings<QWidget>(comboBox);
}

/**************************************************
*                                                 *
*               Template MainWindow               *
*                                                 *
***************************************************/

template <>
inline void Settings::ReadSettings(QMainWindow* mainWindow)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(mainWindow->objectName());
    mainWindow->restoreGeometry(settings.value("geometry").toByteArray());
    mainWindow->restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();
    ReadSettings<QWidget>(mainWindow);
}

template <>
inline void Settings::WriteSettings(QMainWindow* mainWindow)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(mainWindow->objectName());
    settings.setValue("geometry", mainWindow->saveGeometry());
    settings.setValue("windowState", mainWindow->saveState());
    settings.endGroup();
    WriteSettings<QWidget>(mainWindow);
}

/**************************************************
*                                                 *
*               Template Checkbox                 *
*                                                 *
***************************************************/

template <>
inline void Settings::ReadSettings(QCheckBox* checkBox)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(checkBox->objectName());

    if (settings.value("checked").toString()!="") {
        checkBox->setChecked(settings.value("checked").toBool());
    }
    settings.endGroup();

    ReadSettings<QWidget>(checkBox);
}

template <>
inline void Settings::WriteSettings(QCheckBox* checkBox)
{
    QSettings settings("Gw2SPECS");
    settings.beginGroup(checkBox->objectName());
    settings.setValue("checked", checkBox->isChecked());
    settings.endGroup();

    WriteSettings<QWidget>(checkBox);
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
