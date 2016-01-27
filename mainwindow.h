#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAINWINDOW_WEBSITE_URL "http://gw2dmg-meter.de/howto/index.html"

#include <QMainWindow>
#include <QThread>
#include "configurator.h"
#include <QTcpSocket>

namespace Ui
{
    class MainWindow;
}

namespace GW2
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void EnableTransparency(bool isAlmostTransparent);
        void LinkToWebsite();
        void UpdateTime(int timeInMsecs);
        void UpdateDmg(unsigned long long dmg);
        void UpdateDps(int dps);
        void UpdateMaxDmg(int maxDmg);

    private:
        Ui::MainWindow *ui;
        QThread m_ScreenRecorderThread;
        Configurator m_Configurator;
        QTcpSocket *socket;
    };
    extern int m_Dps;
}


#endif // MAINWINDOW_H
