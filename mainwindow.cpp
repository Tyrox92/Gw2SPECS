#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_configurator.h"
#include <QUrl>
#include <QDesktopServices>
#include <QtDebug>
#include "screenrecorder.h"
#include "settings.h"
#include "global.h"
#include <QSizeGrip>
#include <QToolBar>
#include <QToolButton>
#include "ui_mydialog.h"
#include <QtNetwork>
#include <QUrl>

using namespace GW2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Configurator(this),
    m_MyDialog(this),
    update_Timer(this)
{
    ui->setupUi(this);
    StartupPref();

    QObject::connect(&update_Timer, SIGNAL(timeout()), this, SLOT(UpdateTimer()));

    ScreenRecorder* screenRecorder = new ScreenRecorder;
    DmgMeter* dmgMeter = &screenRecorder->GetDmgMeter();
    screenRecorder->moveToThread(&m_ScreenRecorderThread);

    Ui::Configurator* uiConfig = m_Configurator.ui;

    dmgMeter->moveToThread(&m_ScreenRecorderThread);

    QObject::connect(&m_ScreenRecorderThread, SIGNAL(finished()), screenRecorder, SLOT(deleteLater()));
    QObject::connect(&m_ScreenRecorderThread, SIGNAL(started()), screenRecorder, SLOT(StartRecording()));
    QObject::connect(screenRecorder, SIGNAL(RequestInfoUpdate(QString)), ui->labelInfo, SLOT(setText(QString)));
    QObject::connect(dmgMeter, SIGNAL(RequestTimeUpdate(int)), this, SLOT(UpdateTime(int)));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), dmgMeter, SLOT(Reset()));
    QObject::connect(ui->actionAutoReset, SIGNAL(triggered(bool)), dmgMeter, SLOT(SetIsAutoResetting(bool)));
    QObject::connect(ui->actionEnableTransparency, SIGNAL(triggered(bool)), this, SLOT(EnableTransparency(bool)));
    QObject::connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(LinkToWebsite()));
    QObject::connect(ui->actionConfig, SIGNAL(triggered()), &m_Configurator, SLOT(exec()));
    // connecting configurator - solo display settings
    QObject::connect(uiConfig->checkBoxSoloProfColors, SIGNAL(clicked(bool)), this, SLOT(SProfSettingsChanged()));
    QObject::connect(uiConfig->professionSoloComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(SProfChanged(QString)));
    QObject::connect(uiConfig->checkBoxSoloName, SIGNAL(clicked(bool)), this, SLOT(SNameChanged()));
    QObject::connect(uiConfig->checkBoxSoloDamage, SIGNAL(clicked(bool)), this, SLOT(SDamageChanged()));
    QObject::connect(uiConfig->checkBoxSoloDPS, SIGNAL(clicked(bool)), this, SLOT(SDPSChanged()));
    QObject::connect(uiConfig->checkBoxSoloCDamage, SIGNAL(clicked(bool)), this, SLOT(SCDamageChanged()));
    QObject::connect(uiConfig->checkBoxSoloCPerDmg, SIGNAL(clicked(bool)), this, SLOT(SCPerDmgChanged()));
    QObject::connect(uiConfig->checkBoxSoloCDPS, SIGNAL(clicked(bool)), this, SLOT(SCDPSChanged()));
    // connecting configurator - group display settings
    QObject::connect(uiConfig->checkBoxGroupProfColors, SIGNAL(clicked(bool)), this, SLOT(GProfSettingsChanged()));
    QObject::connect(uiConfig->professionComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(GProfChanged(QString)));
    QObject::connect(uiConfig->checkBoxGroupPosition, SIGNAL(clicked(bool)), this, SLOT(GPositionChanged()));
    QObject::connect(uiConfig->checkBoxGroupName, SIGNAL(clicked(bool)), this, SLOT(GNameChanged()));
    QObject::connect(uiConfig->checkBoxGroupDamage, SIGNAL(clicked(bool)), this, SLOT(GDamageChanged()));
    QObject::connect(uiConfig->checkBoxGroupPerDmg, SIGNAL(clicked(bool)), this, SLOT(GPerDmgChanged()));
    QObject::connect(uiConfig->checkBoxGroupDPS, SIGNAL(clicked(bool)), this, SLOT(GDPSChanged()));
    QObject::connect(uiConfig->checkBoxGroupActivity, SIGNAL(clicked(bool)), this, SLOT(GActivityChanged()));
    // connecting configurator - accuracy settings
    QObject::connect(uiConfig->comboBoxScreenshots, SIGNAL(currentIndexChanged(QString)), screenRecorder, SLOT(SetScreenshotsPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxUpdates, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetUpdatesPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxSecondsInCombat, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetSecondsInCombat(QString)));
    QObject::connect(uiConfig->comboBoxConsideredLines, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetConsideredLineCount(QString)));
    QObject::connect(uiConfig->pushButtonReset, SIGNAL(clicked(bool)), &m_Configurator, SLOT(RestoreDefaults()));
    // context menu

    myMenu.setStyleSheet("QMenu{background-color: rgb(32, 43, 47);color:#f2f2f2;}QMenu::item:selected{background-color: rgb(52, 63, 67);}");
    subMenu->setStyleSheet("QMenu{background-color: rgb(32, 43, 47);color:#f2f2f2;}QMenu::item:selected{background-color: rgb(52, 63, 67);}");

    exitSeparator->setSeparator(true);
    myMenu.addAction(exitSeparator);

    exitMenu->setIcon(QIcon(":/Exit"));
    exitMenu->setIconVisibleInMenu(true);
    QObject::connect(exitMenu, SIGNAL(triggered(bool)), this, SLOT(close()));

    resetData->setIcon(QIcon(":/Reset"));
    resetData->setIconVisibleInMenu(true);
    QObject::connect(resetData, SIGNAL(triggered()), dmgMeter, SLOT(Reset()));

    extraOptions->setCheckable(true);
    extraOptions->setIcon(QIcon(":/moreDetails"));
    extraOptions->setIconVisibleInMenu(true);
    QObject::connect(extraOptions, SIGNAL(triggered(bool)), this, SLOT(on_actionActionGroupDetails_toggled(bool)));

    transparentWindow->setCheckable(true);
    transparentWindow->setIcon(QIcon(":/Transparency"));
    transparentWindow->setIconVisibleInMenu(true);
    QObject::connect(transparentWindow, SIGNAL(triggered(bool)), this, SLOT(EnableTransparency(bool)));

    autoReset->setCheckable(true);
    autoReset->setIcon(QIcon(":/Auto_Reset"));
    autoReset->setIconVisibleInMenu(true);
    QObject::connect(autoReset, SIGNAL(triggered(bool)), dmgMeter, SLOT(SetIsAutoResetting(bool)));
    QObject::connect(autoReset, SIGNAL(triggered(bool)), this, SLOT(resetAutomatic(bool)));

    connectServer->setCheckable(true);
    connectServer->setIcon(QIcon(":/connect"));
    connectServer->setIconVisibleInMenu(true);
    QObject::connect(connectServer, SIGNAL(triggered()), this, SLOT(on_actionConnect_triggered()));
    QObject::connect(connectServer, SIGNAL(triggered(bool)), this, SLOT(connectToServ(bool)));


    saveToFile->setIcon(QIcon(":/saveIcon"));
    saveToFile->setIconVisibleInMenu(true);
    QObject::connect(saveToFile, SIGNAL(triggered()), this, SLOT(on_actionActionSave_triggered()));

    options->setIcon(QIcon(":/Config"));
    options->setIconVisibleInMenu(true);
    QObject::connect(options, SIGNAL(triggered()), &m_Configurator, SLOT(exec()));

    hideShowToolbar->setCheckable(true);
    QObject::connect(hideShowToolbar, SIGNAL(toggled(bool)), this, SLOT(HideAndShowToolbar(bool)));

    myMenu.addMenu(subMenu);

    ui->scrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->widget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->scrollArea, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu(const QPoint&)));
    QObject::connect(ui->widget, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenuDetails(const QPoint&)));

    QString readToolbar = ReadToolbarSettings();
    if(readToolbar == "hidden"){
        HideAndShowToolbar(true);
        hideShowToolbar->setChecked(true);
    }else{
        ui->toolBar->show();
    }


    // reset button
    QObject::connect(ui->pushButton, SIGNAL(toggled(bool)), this, SLOT(on_pushButton_toggled(bool)));

    dmgMeter->SetUpdatesPerSecond(uiConfig->comboBoxUpdates->currentText());
    dmgMeter->SetSecondsInCombat(uiConfig->comboBoxSecondsInCombat->currentText());
    dmgMeter->SetConsideredLineCount(uiConfig->comboBoxConsideredLines->currentText());
    dmgMeter->Reset(); //Make sure Everything is reset if not set Timer will be set to 2s upon start

    m_ScreenRecorderThread.start();

    //Settings::ReadSettings<QMainWindow>(this);

    // Start screenshot timer from separate thread
    const int oldIndex = uiConfig->comboBoxScreenshots->currentIndex();
    uiConfig->comboBoxScreenshots->setCurrentIndex((uiConfig->comboBoxScreenshots->currentIndex() + 1) % uiConfig->comboBoxScreenshots->count());
    uiConfig->comboBoxScreenshots->setCurrentIndex(oldIndex);

    // solo settings
    displaySProfColor=uiConfig->checkBoxSoloProfColors->isChecked();
    displaySName=uiConfig->checkBoxSoloName->isChecked();
    displaySDmg=uiConfig->checkBoxSoloDamage->isChecked();
    displaySDPS=uiConfig->checkBoxSoloDPS->isChecked();
    displaySCDmg=uiConfig->checkBoxSoloCDamage->isChecked();
    displaySCPer=uiConfig->checkBoxSoloCPerDmg->isChecked();
    displaySCDPS=uiConfig->checkBoxSoloCDPS->isChecked();
    // group settings
    displayGProfColor=uiConfig->checkBoxGroupProfColors->isChecked();
    displayGPos=uiConfig->checkBoxGroupPosition->isChecked();
    displayGName=uiConfig->checkBoxGroupName->isChecked();
    displayGPer=uiConfig->checkBoxGroupPerDmg->isChecked();
    displayGDmg=uiConfig->checkBoxGroupDamage->isChecked();
    displayGDPS=uiConfig->checkBoxGroupDPS->isChecked();
    displayGAct=uiConfig->checkBoxGroupActivity->isChecked();

    uiConfig->professionComboBox->setCurrentIndex(0);
    m_MyProfession=uiConfig->professionComboBox->currentIndex();
    soloMyProfession=uiConfig->professionSoloComboBox->currentIndex();

    // We are not connected on start up
    is_connected = false;

    // generation widgets, labels + styling and aligning the,
    InterfaceGeneration();

    CheckFirstRun();
    CheckForUpdate();
    Initialize();
}

void MainWindow::InterfaceGeneration()
{
    // this function is called once in updateTimer at start up

    //New ProgressbarLabels
    QHBoxLayout *layoutlegend = new QHBoxLayout(ui->legend);
    QHBoxLayout *layoutprogressbar_0 = new QHBoxLayout(ui->progressBar_0);
    QHBoxLayout *layoutprogressbar_1 = new QHBoxLayout(ui->progressBar_1);
    QHBoxLayout *layoutprogressbar_2 = new QHBoxLayout(ui->progressBar_2);
    QHBoxLayout *layoutprogressbar_3 = new QHBoxLayout(ui->progressBar_3);
    QHBoxLayout *layoutprogressbar_4 = new QHBoxLayout(ui->progressBar_4);
    QHBoxLayout *layoutprogressbar_5 = new QHBoxLayout(ui->progressBar_5);
    QHBoxLayout *layoutprogressbar_6 = new QHBoxLayout(ui->progressBar_6);
    QHBoxLayout *layoutprogressbar_7 = new QHBoxLayout(ui->progressBar_7);
    QHBoxLayout *layoutprogressbar_8 = new QHBoxLayout(ui->progressBar_8);
    QHBoxLayout *layoutprogressbar_9 = new QHBoxLayout(ui->progressBar_9);

    // styling layouts
    layoutlegend->setContentsMargins(10,3,10,3);
    layoutprogressbar_0->setContentsMargins(10,0,10,0);
    layoutprogressbar_1->setContentsMargins(10,0,10,0);
    layoutprogressbar_2->setContentsMargins(10,0,10,0);
    layoutprogressbar_3->setContentsMargins(10,0,10,0);
    layoutprogressbar_4->setContentsMargins(10,0,10,0);
    layoutprogressbar_5->setContentsMargins(10,0,10,0);
    layoutprogressbar_6->setContentsMargins(10,0,10,0);
    layoutprogressbar_7->setContentsMargins(10,0,10,0);
    layoutprogressbar_8->setContentsMargins(10,0,10,0);
    layoutprogressbar_9->setContentsMargins(10,0,10,0);

    // adding colomns to bars (not yet shown)
    // legend
    layoutlegend->addWidget(labellegendname);
    layoutlegend->addWidget(labellegenddmg);
    layoutlegend->addWidget(labellegendper);
    layoutlegend->addWidget(labellegenddps);
    //labellegend->addWidget(labellegendact);
    // names
    layoutprogressbar_0->addWidget(labelname_0);
    layoutprogressbar_1->addWidget(labelname_1);
    layoutprogressbar_2->addWidget(labelname_2);
    layoutprogressbar_3->addWidget(labelname_3);
    layoutprogressbar_4->addWidget(labelname_4);
    layoutprogressbar_5->addWidget(labelname_5);
    layoutprogressbar_6->addWidget(labelname_6);
    layoutprogressbar_7->addWidget(labelname_7);
    layoutprogressbar_8->addWidget(labelname_8);
    layoutprogressbar_9->addWidget(labelname_9);
    // dmg
    layoutprogressbar_0->addWidget(labeldmg_0);
    layoutprogressbar_1->addWidget(labeldmg_1);
    layoutprogressbar_2->addWidget(labeldmg_2);
    layoutprogressbar_3->addWidget(labeldmg_3);
    layoutprogressbar_4->addWidget(labeldmg_4);
    layoutprogressbar_5->addWidget(labeldmg_5);
    layoutprogressbar_6->addWidget(labeldmg_6);
    layoutprogressbar_7->addWidget(labeldmg_7);
    layoutprogressbar_8->addWidget(labeldmg_8);
    layoutprogressbar_9->addWidget(labeldmg_9);
    // percental dmg
    layoutprogressbar_0->addWidget(labelper_0);
    layoutprogressbar_1->addWidget(labelper_1);
    layoutprogressbar_2->addWidget(labelper_2);
    layoutprogressbar_3->addWidget(labelper_3);
    layoutprogressbar_4->addWidget(labelper_4);
    layoutprogressbar_5->addWidget(labelper_5);
    layoutprogressbar_6->addWidget(labelper_6);
    layoutprogressbar_7->addWidget(labelper_7);
    layoutprogressbar_8->addWidget(labelper_8);
    layoutprogressbar_9->addWidget(labelper_9);
    // dps
    layoutprogressbar_0->addWidget(labeldps_0);
    layoutprogressbar_1->addWidget(labeldps_1);
    layoutprogressbar_2->addWidget(labeldps_2);
    layoutprogressbar_3->addWidget(labeldps_3);
    layoutprogressbar_4->addWidget(labeldps_4);
    layoutprogressbar_5->addWidget(labeldps_5);
    layoutprogressbar_6->addWidget(labeldps_6);
    layoutprogressbar_7->addWidget(labeldps_7);
    layoutprogressbar_8->addWidget(labeldps_8);
    layoutprogressbar_9->addWidget(labeldps_9);
//    //activity
//    layoutprogressbar_0->addWidget(labelact_0);
//    layoutprogressbar_1->addWidget(labelact_1);
//    layoutprogressbar_2->addWidget(labelact_2);
//    layoutprogressbar_3->addWidget(labelact_3);
//    layoutprogressbar_4->addWidget(labelact_4);
//    layoutprogressbar_5->addWidget(labelact_5);
//    layoutprogressbar_6->addWidget(labelact_6);
//    layoutprogressbar_7->addWidget(labelact_7);
//    layoutprogressbar_8->addWidget(labelact_8);
//    layoutprogressbar_9->addWidget(labelact_9);

    // legend allignment and styling
    labellegendname->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labellegenddmg->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labellegendper->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labellegenddps->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //labellegendact->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labellegendname->setStyleSheet("color:white;background:none;/*background-color:red;min-width:113px;*/");
    labellegenddmg->setStyleSheet("color:white;background:none;/*background-color:green;*/min-width:39px;");
    labellegendper->setStyleSheet("color:white;background:none;/*background-color:blue;*/max-width:32px;min-width:32px;");
    labellegenddps->setStyleSheet("color:white;background:none;/*background-color:black;*/max-width:34px;min-width:34px;");
    //labelact[n]->setStyleSheet("color:white;background:none;");

    labellegendname->setText(QString("Name"));
    labellegenddmg->setText(QString("Damage"));
    labellegendper->setText(QString("%Dmg"));
    labellegenddps->setText(QString("DPS"));

    //hide all legend labels by default
    labellegendname->hide();
    labellegenddmg->hide();
    labellegendper->hide();
    labellegenddps->hide();
    //labellegendact->hide();

    // disable progressbar percentage number
    ui->progressBar_0->setTextVisible(false);
    ui->progressBar_1->setTextVisible(false);
    ui->progressBar_2->setTextVisible(false);
    ui->progressBar_3->setTextVisible(false);
    ui->progressBar_4->setTextVisible(false);
    ui->progressBar_5->setTextVisible(false);
    ui->progressBar_6->setTextVisible(false);
    ui->progressBar_7->setTextVisible(false);
    ui->progressBar_8->setTextVisible(false);
    ui->progressBar_9->setTextVisible(false);

    for(int n=0;n<10;n++) {

        // aligning labels
        labelname[n]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        labeldmg[n]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labelper[n]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labeldps[n]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        //labelact[n]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // styling labels
        labelname[n]->setStyleSheet("color:white;background:none;/*background-color:red;min-width:113px;*/");
        labeldmg[n]->setStyleSheet("color:white;background:none;/*background-color:green;*/min-width:39px;");
        labelper[n]->setStyleSheet("color:white;background:none;/*background-color:blue;*/max-width:32px;min-width:32px;");
        labeldps[n]->setStyleSheet("color:white;background:none;/*background-color:black;*/max-width:34px;min-width:34px;");
        //labelact[n]->setStyleSheet("color:white;background:none;");
    }
}

void MainWindow::CheckFirstRun()
{
    //First Run checks
    QString tmp1=Read1stRun();
    if (tmp1!="OK")
    {
        //show a new window with explanation of the correct gw2 settings

        QDialog *dialog1 = new QDialog();
        QHBoxLayout *layout = new QHBoxLayout(dialog1);
        QLabel *label1 = new QLabel(this);
        label1->setText("Welcome to GW2SPECS!\n\nPlease set up the following options in your Guild Wars 2:\n\n -Options/Graphics Options: Interface Size= Small/Normal\n -Options/Graphics Options: Resolution=Windowed Fullscreen\n -Chatbox/options: Text Size=Medium\n -Chatbox/options: Disable Timestamps\n -Chatbox/Combat page/options: enable only : Outgoing Buff Damage+Outgoing Damage+Outgoing Mitigated Damage\n -Make sure your combat log has more then 12+ lines and always visible\n\n Have fun!");
        layout->addWidget(label1);
        layout->setMargin(10);
        //dialog1->setStyleSheet("background:red;");
        dialog1->show();

        Write1stRun("OK");
    }
}

void GW2::MainWindow::CheckForUpdate()
{
    QString curVersion = Settings::s_Version;

    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QUrl data_url("http://www.gw2dps.com/version/version_check.txt");
    QNetworkReply* reply = nam->get(QNetworkRequest(data_url));
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    if (reply->error() != QNetworkReply::NoError)
    {
        // Something went wrong. Error can be retrieved with: reply->error()
        qDebug() << reply->error();
    }
    else
    {
        // Call reply->readAll() and do what you need with the data
        QString ver = reply->readAll();
        qDebug() << ver;
        if(curVersion < ver)
        {
            qDebug() << "You need to Update";
            QDialog *checkUpdate = new QDialog();
            QVBoxLayout *layout = new QVBoxLayout(checkUpdate);
            QPushButton *download = new QPushButton("Get latest Version!", this);
            QPushButton *changelog = new QPushButton("Check the Changelog!", this);
            QLabel *label = new QLabel("<center>Your Version: <strong style='color:red;'>" + curVersion + "</strong></center><center>New Version: <strong style='color:green';>" + ver + "</strong></center><br>" + "A new Version of GW2SPECS is available!", this);

            //Connect Functions to Buttons when clicked
            connect(download, SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
            connect(changelog, SIGNAL(clicked(bool)),this,SLOT(on_pushButton2_clicked()));

            //Add Widgets to the Layout
            layout->addWidget(label);
            layout->addWidget(download);
            layout->addWidget(changelog);

            //Style the Dialog
            layout->setMargin(30);

            checkUpdate->setStyleSheet("background:#f4f4f4;");
            checkUpdate->setWindowFlags(Qt::WindowStaysOnTopHint);

            //Style Other Elements
            download->setStyleSheet("background:#8BB2DA;");

            //Display Dialog
            checkUpdate->show();
        }
    }
}

void MainWindow::Initialize()
{
    if (HostIP != "" && (is_connected == false))
    {
        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(connected()),this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
        connect(socket, SIGNAL(readyRead()),this, SLOT(ready2Read()));
        qDebug() << "connecting to : " << HostIP << ":" << HostPort;

        MyClientSlot=10; //no semi-handshake yet
        CurrentMeta=0;CurrentPos=0;
        int i;
        for (i=0;i<10;i++)
        {
            SlotDmg[i]=0;
            SlotDPS[i]=0;
            SlotAct[i]=0;
            SlotName[i][0]='\0';
        }
        GrpDmg=0;
        GrpDPS=0;
        AvgDPS=0;
        hitCounter=0;
        m_critChance=0;
        critCounter=0;
        m_condiDmg=0;
        LastColor=0;
        combatCourse = "";

        socket->connectToHost(HostIP, HostPort);

        if(!socket->waitForConnected(5000))
        {
            qDebug() << "Error: " << socket->errorString();
            QDialog *dialog = new QDialog();
            QHBoxLayout *layout = new QHBoxLayout(dialog);
            QLabel *label = new QLabel(this);
            label->setText("Connection to " + HostIP + " failed");
            layout->addWidget(label);
            layout->setMargin(50);
            dialog->setStyleSheet("background:red;");
            dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
            dialog->show();
            is_connected = false;
        }
        else is_connected = true;
        //m_MyProfession=0;
        // we need to wait...
        m_Dps=0;m_Dmg=0;m_Activity=0;m_MaxDmg=0;
        update_Timer.start(1000);
    }
    else
    {
        is_connected = false;

        // this is not blocking call
        MyClientSlot=10; //no semi-handshake yet
        CurrentMeta=0;CurrentPos=0;

        int i;
        for (i=0;i<10;i++)
        {
            SlotDmg[i]=0;
            SlotDPS[i]=0;
            SlotAct[i]=0;
            SlotName[i][0]='\0';
        }
        GrpDmg=0;
        hitCounter=0;
        m_critChance=0;
        critCounter=0;
        m_condiDmg=0;
        LastColor=0;
        //m_MyProfession=0;
        // we need to wait...
        m_Dps=0;m_Dmg=0;m_Activity=0;m_MaxDmg=0;
        update_Timer.start(1000);
    }
}

void MainWindow::SProfChanged(QString prof)
{
    QStringList proflist;
    proflist << "Elementalist" << "Engineer" << "Guardian" << "Mesmer" << "Necromancer" << "Ranger" << "Revenant" << "Thief" << "Warrior";
    soloMyProfession = proflist.indexOf(prof)+1;
}
void MainWindow::SProfSettingsChanged()
{
    if (displaySProfColor==1) displaySProfColor=0; else displaySProfColor=1;
}
void MainWindow::SNameChanged()
{
    if (displaySName==1) displaySName=0; else displaySName=1;
}
void MainWindow::SDamageChanged()
{
    if (displaySDmg==1) displaySDmg=0; else displaySDmg=1;
}
void MainWindow::SDPSChanged()
{
    if (displaySDPS==1) displaySDPS=0; else displaySDPS=1;
}
void MainWindow::SCDamageChanged()
{
    if (displaySCDmg==1) displaySDmg=0; else displaySCDmg=1;
}
void MainWindow::SCPerDmgChanged()
{
    if (displaySCPer==1) displaySCPer=0; else displaySCPer=1;
}
void MainWindow::SCDPSChanged()
{
    if (displaySCDPS==1) displaySCDPS=0; else displaySCDPS=1;
}

void MainWindow::GProfChanged(QString prof)
{
    QStringList proflist;
    proflist << "Elementalist" << "Engineer" << "Guardian" << "Mesmer" << "Necromancer" << "Ranger" << "Revenant" << "Thief" << "Warrior";
    m_MyProfession = proflist.indexOf(prof)+1;
}
void MainWindow::GProfSettingsChanged()
{
    if (displayGProfColor==1) displayGProfColor=0; else displayGProfColor=1;
}
void MainWindow::GPositionChanged()
{
    if (displayGPos==1) displayGPos=0; else displayGPos=1;
}
void MainWindow::GNameChanged()
{
    if (displayGName==1) displayGName=0; else displayGName=1;
}
void MainWindow::GDamageChanged()
{
    if (displayGDmg==1) displayGDmg=0; else displayGDmg=1;
}
void MainWindow::GPerDmgChanged()
{
    if (displayGPer==1) displayGPer=0; else displayGPer=1;
}
void MainWindow::GDPSChanged()
{
    if (displayGDPS==1) displayGDPS=0; else displayGDPS=1;
}
void MainWindow::GActivityChanged()
{
    if (displayGAct==1) displayGAct=0; else displayGAct=1;
}

void MainWindow::UpdateGroupLabels()
{
    QProgressBar* Bar0 = ui->progressBar_0;
    QProgressBar* Bar1 = ui->progressBar_1;
    QProgressBar* Bar2 = ui->progressBar_2;
    QProgressBar* Bar3 = ui->progressBar_3;
    QProgressBar* Bar4 = ui->progressBar_4;
    QProgressBar* Bar5 = ui->progressBar_5;
    QProgressBar* Bar6 = ui->progressBar_6;
    QProgressBar* Bar7 = ui->progressBar_7;
    QProgressBar* Bar8 = ui->progressBar_8;
    QProgressBar* Bar9 = ui->progressBar_9;
    QProgressBar* Bar [10] = {Bar0,Bar1,Bar2,Bar3,Bar4,Bar5,Bar6,Bar7,Bar8,Bar9};

    // hide labels
//    for(int n=0;n<10;n++) {
//        labelname[n]->hide();
//        labeldmg[n]->hide();
//        labelper[n]->hide();
//        labeldps[n]->hide();
//        //labelact[n]->hide();
//    }

    long p,i,j,k;

    // If playing without a server
    // Display only the solo user information
    if ((is_connected == false) && MyClientSlot == 10)
    {
        //StartupHideProgressBars();
        PosDmg[0]=m_Dmg;
        PosDPS[0]=m_Dps;
        // PosAct[0]=m_Activity;
        if (PosDmg[0]>0)
            i=PosDmg[0]*100.0/PosDmg[0];
        else i=0;


        Bar[0]->setVisible(true);

        labellegendact->hide();
        labellegendper->hide();
        labelper[0]->hide();
        labelact[0]->hide();

        // set bar length
        Bar[0]->setValue(i);

        //display name
        if (displaySName==true) {
            if (ReadNameSettings()!="") labelname[0]->setText(QString("%1").arg(ReadNameSettings()));
            else labelname[0]->setText(QString("Myself"));
            labelname[0]->show();
            labellegendname->show();
        } else {
            labelname[0]->hide();
            labellegendname->hide();
        }

        // damage
        if (displaySDmg==true) {
            labeldmg[0]->setText(QString("%L1").arg(PosDmg[0]));
            labeldmg[0]->show();
            labellegenddmg->show();
        } else {
            labeldmg[0]->hide();
            labellegenddmg->hide();
        }

        // DPS
        if (displaySDPS==true) {
            labeldps[0]->setText(QString("%L1").arg(PosDPS[0]));
            labeldps[0]->show();
            labellegenddps->show();
        } else {
            labeldps[0]->hide();
            labellegenddps->hide();
        }
    }
    else
    {
        // reset total dmg,dps
        GrpDmg=0;
        GrpDPS=0;

        // add up all dmg/dps and set the labels
        for (j=0;j<10;j++) GrpDmg+=SlotDmg[j];
        ui->grp_Dmg->setText(QString::number(GrpDmg));
        for (j=0;j<10;j++) GrpDPS+=SlotDPS[j];
        ui->grp_DPS->setText(QString::number(GrpDPS));

        // reset number of player; set
        i=0;
        for (j=0;j<10;j++) if (SlotDPS[j]>0) i++;
        if (i>0) AvgDPS=GrpDPS/i; else AvgDPS=0;
        ui->avg_DPS->setText(QString::number(AvgDPS));

        // setting input from server to local Slots
        for (j=0;j<10;j++)
        {
            strcpy(PosName[j],SlotName[j]);
            PosDmg[j]=SlotDmg[j];
            PosDPS[j]=SlotDPS[j];
            //PosAct[j]=SlotAct[j];
            PosProf[j]=SlotProf[j];
        }

        // reseting empty/disconnected slot to 0
        for (int p=0;p<10;p++)
        {
            if (PosName[p]==QString("Disconnected"))
            {
                PosName[p][0]=0;
                PosDmg[p]=0;
                PosDPS[p]=0;
                PosAct[p]=0;
                PosProf[p]=0;
            }
        }

        // sorting the slots
        k=0;
        for (i=0;i<9;i++)
        {
            for (j=i+1;j<10;j++)
            {
                if (PosDmg[j]>PosDmg[i])
                {
                    k=PosDmg[i];
                    PosDmg[i]=PosDmg[j];
                    PosDmg[j]=k;
                    k=PosDPS[i];
                    PosDPS[i]=PosDPS[j];
                    PosDPS[j]=k;
                    //k=PosAct[i];
                    //PosAct[i]=PosAct[j];
                    //PosAct[j]=k;
                    k=PosProf[i];
                    PosProf[i]=PosProf[j];
                    PosProf[j]=k;
                    strcpy(tmp1,PosName[i]);
                    strcpy(PosName[i],PosName[j]);
                    strcpy(PosName[j],tmp1);
                }
            }
        }

        // doing the math and setting the labels
        for(int n=0;n<10;n++) {
            Bar[n]->setVisible(true);
            //Bar[n]->setTextVisible(false);
            if (PosName[n][0]!=0) {
                // le math
                if (PosDmg[0]>0)
                    i=PosDmg[n]*100/PosDmg[0];
                else i=0;
                if (GrpDmg>0)
                    p=PosDmg[n]*100/GrpDmg;
                else p=0;

                // profession based bar coloring
                if (displayGProfColor==true)
                {
                    switch (PosProf[n])
                    {
                    case 0:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(3, 132, 146 , 60%);}");
                        break;
                    case 1:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(236, 87, 82, 70%);}");
                        break;
                    case 2:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(153,102,51, 70%);}");
                        break;
                    case 3:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(51,153,204, 70%);}");
                        break;
                    case 4:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(153,51,153, 70%);}");
                        break;
                    case 5:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(51,153,102, 70%);}");
                        break;
                    case 6:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(102,204,51, 70%);}");
                        break;
                    case 7:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(204,99,66, 70%);}");
                        break;
                    case 8:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(204,102,102, 70%);}");
                        break;
                    case 9:
                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;color: rgb(255, 255, 255);min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(255,153,51, 70%);}");
                        break;
                    }
                }
                else
                    if (n%2==0) Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(3, 132, 146 , 60%);}");
                    else Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(4,165,183, 60%);}");

                // set bar length
                Bar[n]->setValue(i);

                //display name and position or not
                if (displayGName==true) {
                    if (displayGPos==true) labelname[n]->setText(QString("%1. %2").arg(n+1).arg(PosName[n]));
                    else labelname[n]->setText(QString("%1").arg(PosName[n]));
                    labelname[n]->show();
                    labellegendname->show();
                } else {
                    labelname[n]->hide();
                    labellegendname->hide();
                }

                // damage
                if (displayGDmg==true) {
                    labeldmg[n]->setText(QString("%L1").arg(PosDmg[n]));
                    labeldmg[n]->show();
                    labellegenddmg->show();
                } else {
                    labeldmg[n]->hide();
                    labellegenddmg->hide();
                }

                // percental
                if (displayGPer==true) {
                    labelper[n]->setText(QString("%L1%").arg(p));
                    labelper[n]->show();
                    labellegendper->show();
                } else {
                    labelper[n]->hide();
                    labellegendper->hide();
                }

                // DPS
                if (displayGDPS==true) {
                    labeldps[n]->setText(QString("%L1").arg(PosDPS[n]));
                    labeldps[n]->show();
                    labellegenddps->show();
                } else {
                    labeldps[n]->hide();
                    labellegenddps->hide();
                }

                // activity
                //if (displayGAct==true) {
                //    labelact[n]->setText(QString("%L1%").arg());
                //    labelact[n]->show();
                //    labellegendact->show();
                //} else {
                //    labelact[n]->hide();
                //    labellegendact->hide();
                //}
            }
            else
                Bar[n] ->setVisible(false);
        }
    }
    updateCombatCourse();
}

void MainWindow::ready2Read()
{

    int i,j;
    long k;


    incData = socket->readAll();
    incDataSize = incData.size();
    memcpy(incData2, incData.data(), incDataSize);

    i=0;

    if (MyClientSlot==10)
    {
        if ((incDataSize==4) && (incData2[0]=='*') && (incData2[1]=='*') && (incData2[2]=='*'))
        {
            MyClientSlot= incData2[3]-48;

        }
    }
    else
    {
        while (i<incDataSize)
        {
            if ((incData2[i]=='*') && (incData2[i+3]=='#'))
            {

                if ((incData2[i+1]<58) && (incData2[i+1]>47) && (incData2[i+2]>48) && (incData2[i+2]<54))
                {
                    CurrentPos=incData2[i+1]-48;
                    CurrentMeta=incData2[i+2]-48;
                }
                i+=3;

            }
            else if (incData2[i]=='#')
            {
                if (CurrentMeta==1)
                {

                    j=i+1;
                    while ((j-i-1<13) && (j<incDataSize) && (incData2[j]!='*')) { SlotName[CurrentPos][j-i-1]=incData2[j];j++; }
                    if (incData2[j]=='*') SlotName[CurrentPos][j-i-1]=0; else SlotName[CurrentPos][0]=0;
                    i=j;

                }else
                {



                    j=i+1;k=0;
                    while ((j-i-1<12) && (j<incDataSize) && (incData2[j]!='*')&& (incData2[j]>47)&& (incData2[j]<58)) { k=k*10+incData2[j]-48;j++; }
                    if  (incData2[j]=='*')
                    {

                        if  (CurrentMeta==2) SlotDPS[CurrentPos]=k;
                        if  (CurrentMeta==3) SlotDmg[CurrentPos]=k;
                        if  (CurrentMeta==4) SlotAct[CurrentPos]=k;
                        if  (CurrentMeta==5) SlotProf[CurrentPos]=k;

                    }
                    i=j;
                }
            }
            else { i++; while((i<incDataSize) && (incData2[i])!='*') i++; }

        }

    }


}

void MainWindow::connected()
{
    qDebug() << "connected...";
    MyClientSlot=10;  //no handshake yet
}

void MainWindow::disconnected()
{
    is_connected = false;MyClientSlot=10;
    qDebug() << "disconnected...";
    //so what now? exit?
}

MainWindow::~MainWindow()
{
    m_ScreenRecorderThread.quit();
    Settings::WriteSettings<QMainWindow>(this);
    if (!m_ScreenRecorderThread.wait(1000))
    {
        // Failed to exit thread => terminate it.
        m_ScreenRecorderThread.terminate();
        m_ScreenRecorderThread.wait();
    }

    delete ui;
}

void MainWindow::EnableTransparency(bool isAlmostTransparent)
{
    if (isAlmostTransparent)
    {
        this->ui->centralWidget->setStyleSheet("background-color: rgba(32, 43, 47, 0%);");
        ui->toolBar->setStyleSheet("QWidget { background-color: rgba(32, 43, 47, 1%); } QToolButton { background-color: rgba(32, 43, 47, 1%); }");
        ui->grp_DPS->setStyleSheet("");
        transparentWindow->setText("Transparency Off");
        this->show();
    }
    else
    {
        this->ui->centralWidget->setStyleSheet("background-color: rgba(32, 43, 47, 60%);");
        ui->toolBar->setStyleSheet("QWidget { background-color: rgba(32, 43, 47, 60%); } QToolButton { background-color: rgba(32, 43, 47, 1%); }");
        ui->grp_DPS->setStyleSheet("");
        transparentWindow->setText("Transparency On");
        this->show();
    }
}

void MainWindow::LinkToWebsite()
{
    QDesktopServices::openUrl(QUrl(MAINWINDOW_WEBSITE_URL));
}

void MainWindow::SendClientInfo(void)
{
    QByteArray tmp1;
    const char* tmp2;

    tmp1 = MyName.toLatin1();
    tmp2 = tmp1.data();
    if (MyClientSlot!=10)  //connected and semi-handshaked
    {
        //Failsafe Checks
        if (m_Dps>99999) m_Dps = 1;
        if (m_Dmg>999999999) m_Dmg = 1;
        if (m_Activity>100) m_Activity = 1;

        sprintf(writeBuff, "*%u1#%s*%u2#%lu*%u3#%lu*%u4#%lu*%u5#%lu*", MyClientSlot, tmp2 , MyClientSlot, m_Dps, MyClientSlot, m_Dmg, MyClientSlot, m_Activity,MyClientSlot, m_MyProfession);
        socket->write(writeBuff);
    }
}

void MainWindow::UpdatePersonalLabels()
{
    QLabel* Label1;
    unsigned long c;
    double c1,c2,c3,c4;

    if ((is_connected == true))
    {
        ui->grp_DPS->setStyleSheet("color: rgb(255, 255, 255);");
    }
    else
    {
        Label1 = ui->grp_DPS;
        if (m_Dps > DMGMETER_HIGH_DPS_LIMIT)
        {
            Label1->setStyleSheet(DmgMeter::s_UltraStyle);
        }
        else if (m_Dps > DMGMETER_NORMAL_DPS_LIMIT)
        {
            Label1->setStyleSheet(DmgMeter::s_HighStyle);
        }
        else if (m_Dps > DMGMETER_LOW_DPS_LIMIT)
        {
            Label1->setStyleSheet(DmgMeter::s_NormalStyle);
        }
        else
        {
            Label1->setStyleSheet(DmgMeter::s_LowStyle);
        }
    }
    //Personal Crit Chance Value
    ui->critChance->setText(QString::number(m_critChance));
    //Personal Condi DMG Value
    ui->labelCondiDMGValue->setText(QString::number(m_condiDmg));
    //Personal Condi DPS Value
    c2=m_condiDmg;
    c3=m_Dps;
    c4=m_Dmg;
    c1=c2*c3;
    if (m_Dmg>0)c=round(c1/c4);else c=0;
    Label1 = ui->condiDPS;
    Label1->setText(QString::number(c));
    //Personal Max Damage Value
    Label1 = ui->labelMaxDmgValue;
    Label1->setText(QString::number(m_MaxDmg));
    if (m_MaxDmg > DMGMETER_HIGH_DPS_LIMIT)
    {
        Label1->setStyleSheet(DmgMeter::s_UltraStyle);
    }
    else if (m_MaxDmg > DMGMETER_NORMAL_DPS_LIMIT)
    {
        Label1->setStyleSheet(DmgMeter::s_HighStyle);
    }
    else if (m_MaxDmg > DMGMETER_LOW_DPS_LIMIT)
    {
        Label1->setStyleSheet(DmgMeter::s_NormalStyle);
    }
    else
    {
        Label1->setStyleSheet(DmgMeter::s_LowStyle);
    }
}

void MainWindow::UpdateTimer(void)
{
    // Always on top fix
    this->raise();
    myMenu.raise();
    subMenu->raise();

    if ((is_connected == true))
    {
        ui->actionConnect->setIcon(QIcon(":/connected"));
        connectServer->setIcon(QIcon(":/connected"));
        connectServer->setText("Disconnect");
        SendClientInfo();
    }
    else{
        ui->actionConnect->setIcon(QIcon(":/connect"));
        connectServer->setIcon(QIcon(":/connect"));
        connectServer->setText("Connect");
    }
    UpdateGroupLabels();
    UpdatePersonalLabels();
}

void MainWindow::UpdateTime(int timeInMsecs)
{
    int secs = timeInMsecs / 1000;
    int mins = secs / 60;
    secs -= mins * 60;
    int hours = mins / 60;
    mins -= hours * 60;

    QString time;

    if (hours > 0)
    {
        time += QString::number(hours) + "h ";
    }

    if (mins > 0)
    {
        time += QString::number(mins) + "m ";
    }

    time += QString::number(secs) + "s";
    ui->labelTimeValue->setText(time);
    m_Time.sprintf("%02d:%02d:%02d", hours, mins, secs);
}

// Give movement access to MainWindow
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// Give movement access to MainWindow
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

//Shrink UI to ToolBar
void GW2::MainWindow::on_actionShrinkUI_triggered(bool checked)
{
    if (checked)
    {
        ui->centralWidget->hide();
    }
    else
    {
        ui->centralWidget->show();
    }
}

//Show Condi DPS/Crit%/Condi DMG/Highest Hit if ". . ." icon is actionActionGroupDetails is toggled on/off.
bool GW2::MainWindow::on_pushButton_toggled(bool toggled)
{
    if (toggled)
    {
        ui->widgetExtraDetails->show();
        toggled = true;
    }
    else
    {
        ui->widgetExtraDetails->hide();
        toggled = false;
    }
    return toggled;
}

void GW2::MainWindow::StartupPref()
{
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //ui->toolBar->setWindowFlags(Qt::WindowStaysOnTopHint);
    //ui->toolBar->setAttribute(Qt::WA_TranslucentBackground);
    ui->widget->hide();
    ui->widgetExtraDetails->hide();

    // Resize Option
    // Using gridLayout here which is the main layout
    QSizeGrip *sizeGripRight = new QSizeGrip(this);
    QSizeGrip *sizeGripLeft = new QSizeGrip(this);
    ui->gridLayout_2->addWidget(sizeGripRight, 0,0,10,10,Qt::AlignBottom | Qt::AlignRight);
    ui->gridLayout_2->addWidget(sizeGripLeft, 0,0,10,10,Qt::AlignBottom | Qt::AlignLeft);
    sizeGripLeft->setStyleSheet("background: url(''); width: 20px; height: 20px;");
    sizeGripRight->setStyleSheet("background: url(''); width: 20px; height: 20px;");

    ui->progressBar_0->setVisible(false);
    ui->progressBar_1->setVisible(false);
    ui->progressBar_2->setVisible(false);
    ui->progressBar_3->setVisible(false);
    ui->progressBar_4->setVisible(false);
    ui->progressBar_5->setVisible(false);
    ui->progressBar_6->setVisible(false);
    ui->progressBar_7->setVisible(false);
    ui->progressBar_8->setVisible(false);
    ui->progressBar_9->setVisible(false);
}

//Show player/group details
bool GW2::MainWindow::on_actionActionGroupDetails_toggled(bool toggled)
{
    // Check if this user is playing on a server or not
    if ((is_connected == true))
    {
        if (toggled)
        {
            ui->avg_DPS->show();
            ui->grp_DPS->show();
            ui->grp_Dmg->show();
            ui->labelDmg_2->show();
            ui->labelDmg_3->show();
            ui->labelDmg_3->setText("Grp DPS");
            ui->labelDmg_4->show();
            MainWindow::ui->widget->show();
            extraOptions->setText("Hide Details");
            toggled = true;
        }
        else
        {
            ui->widget->hide();
            extraOptions->setText("Show Details");
            toggled = false;
        }
    }
    // If playing solo - show only DPS/DMG/TIME
    else
    {
        if (toggled)
        {
            ui->avg_DPS->hide();
            ui->grp_DPS->hide();
            ui->labelDmg_3->hide();
            ui->labelDmg_4->hide();
            ui->grp_Dmg->hide();
            ui->labelDmg_2->hide();
            MainWindow::ui->widget->show();
            extraOptions->setText("Hide Details");
            toggled = true;
        }
        else
        {
            ui->widget->hide();
            extraOptions->setText("Show Details");
            toggled = false;
        }
    }
    return toggled;
}

void GW2::MainWindow::on_actionConnect_triggered()
{
    Ui::Configurator* uiConfig = m_Configurator.ui;

    if (ui->actionActionGroupDetails->isChecked())
        ui->actionActionGroupDetails->setChecked(false);
    if (ui->pushButton->isChecked())
        ui->pushButton->setChecked(false);

    // If not connected to a server when triggered
    // Then open myDialog
    if ((is_connected == false))
    {
        update_Timer.stop();
        MyDialog mDialog;
        mDialog.setModal(true);

        mDialog.exec();

        MyName=mDialog.getName();
        HostIP = mDialog.getIP();
        HostPort=mDialog.getPort().toInt();
        m_MyProfession=mDialog.getProfession();
        uiConfig->professionComboBox->setCurrentIndex(m_MyProfession);
        Initialize();
    }
    // Otherwise stop the timer and abort the connection
    else
    {
        update_Timer.stop();
        socket->abort();
        ui->progressBar_0->setVisible(false);
        ui->progressBar_1->setVisible(false);
        ui->progressBar_2->setVisible(false);
        ui->progressBar_3->setVisible(false);
        ui->progressBar_4->setVisible(false);
        ui->progressBar_5->setVisible(false);
        ui->progressBar_6->setVisible(false);
        ui->progressBar_7->setVisible(false);
        ui->progressBar_8->setVisible(false);
        ui->progressBar_9->setVisible(false);
        is_connected = false;
        HostIP="";

        //Go back to the initializer
        Initialize();
    }
}

void GW2::MainWindow::on_actionClose_triggered()
{
    // Let's abort the connection to the server before closing
    if ((is_connected == true)) socket->abort();
}

void GW2::MainWindow::on_pushButton_clicked(){
    QString downloadlink = "http://gw2dps.com/download";
    QDesktopServices::openUrl(QUrl(downloadlink));
}

void GW2::MainWindow::on_pushButton2_clicked(){
    QString changeloglink = "http://gw2dps.com/changelog";
    QDesktopServices::openUrl(QUrl(changeloglink));
}

void MainWindow::updateCombatCourse()
{
    if(countCombat > 0){
        //Filling Up Variable For saving later into the Log
        combatCourse += m_Time + " | " + QString::number(m_Dps) + " | " + QString::number(m_Dmg) + "\r\n";
    }
}

void MainWindow::writeFile(QString separator){
    QString tableSep = "";
    QString fileEnding = "txt";
    if(separator == ";"){
        //If we're in CSV Mode
        tableSep = separator;
        fileEnding = QString::fromLatin1("csv");
    }else{
        //If we're in TXT Mode
        tableSep = "|";
        fileEnding = QString::fromLatin1("txt");
    }

    QDir dir("combatLog/");
    if (!dir.exists()) {dir.mkpath(".");}

    unsigned long c;
    double c1,c2,c3,c4;
    c2=m_condiDmg;
    c3=m_Dps;
    c4=m_Dmg;
    c1=c2*c3;
    if (m_Dmg>0)c=round(c1/c4);else c=0;

    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy-MM-dd_hh''mm''ss"));
    const QString filename = QString::fromLatin1("combatLog/combatLog-%1.%2").arg(timestamp).arg(fileEnding);

    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        if(separator == ": "){
            stream << "*************************************\r\n";
            stream << "*         SPECS Log - v 1.0         *\r\n";
            stream << "*************************************\r\n";
            stream << "\r\n";
        }
        if(separator == ";"){
            stream << "Personal Data\r\n";
        }

        stream << "Time" << separator << m_Time << "\r\n";
        stream << "Hitcounter" << separator << hitCounter << "\r\n";
        stream << "Highest Hit" << separator << m_MaxDmg << "\r\n";
        stream << "CritChance" << separator << m_critChance << "%" << "\r\n";
        stream << "\r\n";
        stream << "DPS" << separator << m_Dps << "\r\n";
        stream << "DMG" << separator << m_Dmg << "\r\n";
        stream << "\r\n";
        stream << "CondiDPS" << separator << c << "\r\n";
        stream << "CondiDMG" << separator << m_condiDmg << "\r\n";
        stream << "\r\n\r\n";

        if(is_connected == true){
            if(separator == ": "){
                stream << "*************************************\r\n";
                stream << "*            Group Info             *\r\n";
                stream << "*************************************\r\n";
            }
            stream << "\r\n";
            stream << "AvgDPS" << separator << AvgDPS << "\r\n";
            stream << "GroupDPS" << separator << GrpDPS << "\r\n";
            stream << "GroupDMG " << separator << GrpDmg << "\r\n";
            stream << "\r\n\r\n\r\n";
            QStringList profList;
            profList << "None" << "Elementalist" << "Engineer" << "Guardian" << "Mesmer" << "Necromancer" << "Ranger" << "Revenant" << "Thief" << "Warrior";
            stream <<  "Name       " << tableSep << "   DPS  " << tableSep << "    DMG    " << tableSep << " Class\r\n";
            for(int i=0;i<10;i++){
                if(PosName[i][0] !=0){
                    QString tempName = PosName[i];
                    QString tempDPS = QString::number(PosDPS[i]);
                    QString tempDMG = QString::number(PosDmg[i]);
                    QString modDMG;

                    if (tempDMG.size() > 3) {
                        modDMG = tempDMG.rightJustified(7, ' ');
                    } if (tempDMG.size() > 6) {
                        modDMG = tempDMG.rightJustified(7, ' ');
                    } else {
                        modDMG = tempDMG.rightJustified(8, ' ');
                    }

                    if (tempDMG.size() > 3) {
                    modDMG.insert(modDMG.size() - 3, ".");
                    }
                    if (tempDMG.size() > 6) {
                    modDMG.insert(modDMG.size() - 7, ".");
                    }


                    if(tempDPS.size() > 3){tempDPS.insert(tempDPS.size() -3, ".");}
                    stream << tempName.leftJustified(10, ' ') << " " << tableSep << " " << tempDPS.rightJustified(6, ' ') << " " << tableSep << " " << modDMG << " " << tableSep << " " << profList[PosProf[i]] << "\r\n";
                }

            }
            stream << "\r\n\r\n\r\n";
        }
        stream << " Time(hh:mm:ss) " << tableSep << "  DPS  " << tableSep << "  DMG\r\n";
        if(separator == ";"){
            combatCourse.replace(QString("|"),QString(";"));
        }
        combatCourse.replace(QString("m "),QString(":"));
        combatCourse.replace(QString("s"),QString(""));
        stream << combatCourse;
    }
}

void GW2::MainWindow::on_actionActionSave_triggered()
{
    //Show Popup here to choose between TXT and CSV
        QDialog *savePopup= new QDialog();
        QVBoxLayout *layout = new QVBoxLayout(savePopup);
        QPushButton *txtButton = new QPushButton(this);
        QPushButton *csvButton = new QPushButton(this);
        QLabel *chooseLabel = new QLabel(this);

        layout->addWidget(chooseLabel);
        layout->addWidget(txtButton);
        layout->addWidget(csvButton);

        layout->setMargin(10);
        chooseLabel->setText("Choose the File Format you like!");
        txtButton->setText("*.txt");
        csvButton->setText("*.csv");

        savePopup->setWindowTitle("Gw2SPECS Save v1.0");
        savePopup->setStyleSheet("background:#f2f2f2;");
        savePopup->setWindowFlags(Qt::WindowStaysOnTopHint| Qt::WindowCloseButtonHint);
        savePopup->show();

        //need to create 2 Buttons
        //set dialog settings to always on top frameless
        //connect Buttons to functions
        QObject::connect(txtButton, SIGNAL(clicked(bool)), this, SLOT(writeTxt()));
        QObject::connect(txtButton, SIGNAL(clicked(bool)), savePopup, SLOT(hide()));
        QObject::connect(csvButton, SIGNAL(clicked(bool)), this, SLOT(writeCsv()));
        QObject::connect(csvButton, SIGNAL(clicked(bool)), savePopup, SLOT(hide()));

}

void GW2::MainWindow::writeTxt()
{
    QString txtSep = ": ";
    writeFile(txtSep);
}

void GW2::MainWindow::writeCsv()
{
    QString csvSep = ";";
    writeFile(csvSep);
}

void GW2::MainWindow::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->scrollArea->viewport()->mapToGlobal(pos);
    myMenu.exec(globalPos);
}

void GW2::MainWindow::ShowContextMenuDetails(const QPoint& pos)
{
    QPoint globalPos = ui->widget->mapToGlobal(pos);
    myMenu.exec(globalPos);
}

bool GW2::MainWindow::HideAndShowToolbar(bool toggled)
{
    qDebug()<< toggled;
    if (toggled)
    {
        //Toolbar is hidden
        WriteToolbarSettings("hidden");
        hideShowToolbar->setText("Show Toolbar");
        ui->toolBar->hide();
        toggled = true;
    }
    else
    {
        //Toolbar is visible
        WriteToolbarSettings("visible");
        hideShowToolbar->setText("Hide Toolbar");
        ui->toolBar->show();
        toggled = false;
    }
    return toggled;
}

bool GW2::MainWindow::connectToServ(bool toggled){
    if (toggled)
    {
        connectServer->setIcon(QIcon(":/connected"));
        connectServer->setText("Disconnect");
        toggled = true;
    }
    else
    {
        connectServer->setIcon(QIcon(":/connect"));
        connectServer->setText("Connect");
        toggled = false;
    }
    return toggled;
}

bool GW2::MainWindow::resetAutomatic(bool toggled){
    if (toggled)
    {
        autoReset->setIcon(QIcon(":/Auto_Reset_Active"));
        autoReset->setText("Auto Reset Off");
        toggled = true;
    }
    else
    {
        autoReset->setIcon(QIcon(":/Auto_Reset"));
        autoReset->setText("Auto Reset On");
        toggled = false;
    }
    return toggled;
}
