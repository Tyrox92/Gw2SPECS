#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAINWINDOW_WEBSITE_URL "http://gw2specs.com"

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
#include <QPoint>
#include <QMenu>
#include <QProgressBar>
#include "qcustomplot.h"

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
        QString time;
        QMenu myMenu;
        QMenu *subMenu = new QMenu("Toggle On/Off", this);
        QMenu *graphMenu = new QMenu("Toggle Graph Settings", this);

        QAction *resetData = myMenu.addAction("Reset");
        QAction *connectServer = myMenu.addAction("Connect");
        QAction *saveToFile = myMenu.addAction("Save File");
        QAction *options = myMenu.addAction("Options");
        QAction *fixOnTop = myMenu.addAction("Fix onTop");
        QAction *exitSeparator = new QAction(this);
        QAction *exitMenu = myMenu.addAction("Exit");


        QAction *hideShowGraph = graphMenu->addAction("Hide Graph"); //Toggle
        QAction *hideShowRealDPS = graphMenu->addAction("Hide RealDPS");

        QAction *hideShowToolbar = subMenu->addAction("Hide Toolbar"); //Toggle
        QAction *extraOptions = subMenu->addAction("Show Details"); //Toggle
        QAction *transparentWindow = subMenu->addAction("Transparency On"); //Toggle
        QAction *autoReset = subMenu->addAction("Auto-Reset On"); //Toggle

        int m_msecs;
        QList<int> _kc;
        int _pos;


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
    int fixOnTopCount;

    QTcpSocket *socket;

    QString MyName;
    QString HostIP;
    int HostPort;

    char PosName[10][15];
    int PosDPS[10];
    long PosDmg[10];
    int PosAct[10];
    int PosProf[10];
    int PosrDPS[10];

    char SlotName[10][15];
    int SlotDPS[10];
    long SlotDmg[10];
    int SlotAct[10];
    int SlotProf[10];
    int SlotrDPS[10];


    long GrpDmg;
    int GrpDPS;
    int AvgDPS;


    int GrprDPS;
    int AvgrDPS;


    QByteArray incData;
    int incDataSize;
    char incData2[800];

    int CurrentPos;
    int CurrentMeta;

    char writeBuff[128];
    bool is_connected;

    // solo settings
    bool displaySProfColor;
    bool displaySName;
    bool displaySDmg;
    bool displaySDPS;
    bool displaySCDmg;
    bool displaySCPer;
    bool displaySCDPS;
    bool displaySrDPS;
    // group settings
    bool displayGProfColor;
    bool displayGPos;
    bool displayGName;
    bool displayGDmg;
    bool displayGPer;
    bool displayGDPS;
    bool displayGAct;
    bool displayGrDPS;

    // name labels (in group mode)
    QLabel *labelname_0 = new QLabel(this);
    QLabel *labelname_1 = new QLabel(this);
    QLabel *labelname_2 = new QLabel(this);
    QLabel *labelname_3 = new QLabel(this);
    QLabel *labelname_4 = new QLabel(this);
    QLabel *labelname_5 = new QLabel(this);
    QLabel *labelname_6 = new QLabel(this);
    QLabel *labelname_7 = new QLabel(this);
    QLabel *labelname_8 = new QLabel(this);
    QLabel *labelname_9 = new QLabel(this);

    // damage dealt labels (in group mode)
    QLabel *labeldmg_0 = new QLabel(this);
    QLabel *labeldmg_1 = new QLabel(this);
    QLabel *labeldmg_2 = new QLabel(this);
    QLabel *labeldmg_3 = new QLabel(this);
    QLabel *labeldmg_4 = new QLabel(this);
    QLabel *labeldmg_5 = new QLabel(this);
    QLabel *labeldmg_6 = new QLabel(this);
    QLabel *labeldmg_7 = new QLabel(this);
    QLabel *labeldmg_8 = new QLabel(this);
    QLabel *labeldmg_9 = new QLabel(this);

    // percental damage dealt in relation to total damage labels (in group mode)
    QLabel *labelper_0 = new QLabel(this);
    QLabel *labelper_1 = new QLabel(this);
    QLabel *labelper_2 = new QLabel(this);
    QLabel *labelper_3 = new QLabel(this);
    QLabel *labelper_4 = new QLabel(this);
    QLabel *labelper_5 = new QLabel(this);
    QLabel *labelper_6 = new QLabel(this);
    QLabel *labelper_7 = new QLabel(this);
    QLabel *labelper_8 = new QLabel(this);
    QLabel *labelper_9 = new QLabel(this);

    // single dps over time labels (in group mode)
    QLabel *labeldps_0 = new QLabel(this);
    QLabel *labeldps_1 = new QLabel(this);
    QLabel *labeldps_2 = new QLabel(this);
    QLabel *labeldps_3 = new QLabel(this);
    QLabel *labeldps_4 = new QLabel(this);
    QLabel *labeldps_5 = new QLabel(this);
    QLabel *labeldps_6 = new QLabel(this);
    QLabel *labeldps_7 = new QLabel(this);
    QLabel *labeldps_8 = new QLabel(this);
    QLabel *labeldps_9 = new QLabel(this);


    // single rdps over time labels (in group mode)
    QLabel *labelrdps_0 = new QLabel(this);
    QLabel *labelrdps_1 = new QLabel(this);
    QLabel *labelrdps_2 = new QLabel(this);
    QLabel *labelrdps_3 = new QLabel(this);
    QLabel *labelrdps_4 = new QLabel(this);
    QLabel *labelrdps_5 = new QLabel(this);
    QLabel *labelrdps_6 = new QLabel(this);
    QLabel *labelrdps_7 = new QLabel(this);
    QLabel *labelrdps_8 = new QLabel(this);
    QLabel *labelrdps_9 = new QLabel(this);


    // activity labels (in group mode)
    QLabel *labelact_0 = new QLabel(this);
    QLabel *labelact_1 = new QLabel(this);
    QLabel *labelact_2 = new QLabel(this);
    QLabel *labelact_3 = new QLabel(this);
    QLabel *labelact_4 = new QLabel(this);
    QLabel *labelact_5 = new QLabel(this);
    QLabel *labelact_6 = new QLabel(this);
    QLabel *labelact_7 = new QLabel(this);
    QLabel *labelact_8 = new QLabel(this);
    QLabel *labelact_9 = new QLabel(this);

    // legend labels (in group mode)
    QLabel *labellegendname = new QLabel(this);
    QLabel *labellegenddmg = new QLabel(this);
    QLabel *labellegendper = new QLabel(this);
    QLabel *labellegenddps = new QLabel(this);
    QLabel *labellegendact = new QLabel(this);
    QLabel *labellegendrdps = new QLabel(this);

    // label arrays
    QLabel* labelname [10] = {labelname_0,labelname_1,labelname_2,labelname_3,labelname_4,labelname_5,labelname_6,labelname_7,labelname_8,labelname_9};
    QLabel* labeldmg [10] = {labeldmg_0,labeldmg_1,labeldmg_2,labeldmg_3,labeldmg_4,labeldmg_5,labeldmg_6,labeldmg_7,labeldmg_8,labeldmg_9};
    QLabel* labelper [10] = {labelper_0,labelper_1,labelper_2,labelper_3,labelper_4,labelper_5,labelper_6,labelper_7,labelper_8,labelper_9};
    QLabel* labeldps [10] = {labeldps_0,labeldps_1,labeldps_2,labeldps_3,labeldps_4,labeldps_5,labeldps_6,labeldps_7,labeldps_8,labeldps_9};
    QLabel* labelact [10] = {labelact_0,labelact_1,labelact_2,labelact_3,labelact_4,labelact_5,labelact_6,labelact_7,labelact_8,labelact_9};
    QLabel* labelrdps [10] = {labelrdps_0,labelrdps_1,labelrdps_2,labelrdps_3,labelrdps_4,labelrdps_5,labelrdps_6,labelrdps_7,labelrdps_8,labelrdps_9};

    char tmp1[20];
    QTimer update_Timer;
    void CheckForUpdate();
    QPushButton *download;
    QPushButton *changelog;
    QString m_Time;

private slots:
    void UpdateTime(int);
    void SendClientInfo();
    void UpdateTimer();
    void UpdateGroupLabels();
    void UpdatePersonalLabels();
    void StartupPref();

    void on_actionShrinkUI_triggered(bool checked);
    bool on_pushButton_toggled(bool toggled);
    bool HideAndShowToolbar(bool);
    bool HideAndShowGraph(bool);
    void Initialize();
    bool on_actionActionGroupDetails_toggled(bool toggled);
    void on_actionConnect_triggered();
    void on_actionClose_triggered();

    void SProfChanged(QString);
    void SProfSettingsChanged();
    void SNameChanged();
    void SDamageChanged();
    void SDPSChanged();
    void SCDamageChanged();
    void SCPerDmgChanged();
    void SCDPSChanged();
    void SrDPSChanged();

    void GProfChanged(QString);
    void GProfSettingsChanged();
    void GPositionChanged();
    void GNameChanged();
    void GDamageChanged();
    void GPerDmgChanged();
    void GDPSChanged();
    void GActivityChanged();
    void GrDPSChanged();

    void SSettingsChanged();
    void GSettingsChanged();
    void CheckFirstRun();
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void writeFile(QString);
    void on_actionActionSave_triggered();
    void updateCombatCourse();
    void writeCsv();
    void writeTxt();
    void ShowContextMenu(const QPoint& pos);
    void ShowContextMenuDetails(const QPoint& pos);
    void ShowContextMenuGraph(const QPoint& pos);
    bool connectToServ(bool);
    bool resetAutomatic(bool);
    //bool hideunhideToolbar(bool toggled);
    void realTimeDataSlot(int dps,int cdps,int avgdps,int msecs,int m_rDps, int m_realDps);
    void runMe();
    void resetGraph();
    bool action_hideShowRealDPS(bool toggled);
    void action_fixOnTop();
    void keyPressEvent( QKeyEvent * event );
};

}


#endif // MAINWINDOW_H
