#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAINWINDOW_WEBSITE_URL "http://gw2dps.com"

#include <QMainWindow>
#include <QThread>
#include "configurator.h"
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include "mydialog.h"
#include <QLabel>

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

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void EnableTransparency(bool isAlmostTransparent);
    void LinkToWebsite();
    /*
        void UpdateTime(int timeInMsecs);
        void UpdateDmg(unsigned long long dmg);
        void UpdateDps();
        void UpdateMaxDmg(int maxDmg);
        */
    void connected();
    void disconnected();
    void ready2Read();


private:
    Ui::MainWindow *ui;
    QThread m_ScreenRecorderThread;
    Configurator m_Configurator;
    MyDialog m_MyDialog;
    QPoint m_dragPosition;

    QTcpSocket *socket;


    QString MyName;
    QString HostIP;
    int HostPort;

    char PosName[10][15];
    int PosDPS[10];
    long PosDmg[10];
    int PosAct[10];
    int PosProf[10];

    char SlotName[10][15];
    int SlotDPS[10];
    long SlotDmg[10];
    int SlotAct[10];
    int SlotProf[10];


    long AllDamageDone;
    int GrpDPS;
    int AvgDPS;

    QByteArray incData;
    int incDataSize;
    char incData2[800];

    int CurrentPos;
    int CurrentMeta;

    char writeBuff[128];
    bool is_connected;

    int ProfBasedColors;
    int pPosition;
    int pDamageDone;
    //QLabel *nameLabel = new QLabel(this);
    //QLabel *dmgLabel = new QLabel(this);

    char tmp1[20];
    QTimer update_Timer;
    void CheckForUpdate();
    QPushButton *download;
    QPushButton *changelog;

private slots:
    void UpdateTime(int);
    void SendClientInfo();
    void UpdateTimer();
    void UpdateGroupLabels();
    void UpdatePersonalLabels();
    void StartupPref();
    void ProfSettingsChanged();
    void ProfChanged(QString);

    void on_actionShrinkUI_triggered(bool checked);
    bool on_pushButton_toggled(bool toggeled);
    void Initialize();
    bool on_actionActionGroupDetails_toggled(bool toggeled);
    void on_actionConnect_triggered();
    void on_actionClose_triggered();
    void PositionChanged();
    void DamageDoneChanged();
    void CheckFirstRun();
    void on_pushButton_clicked();
    void on_pushButton2_clicked();


};

}


#endif // MAINWINDOW_H
