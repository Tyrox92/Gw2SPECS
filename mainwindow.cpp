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
    QObject::connect(uiConfig->comboBoxScreenshots, SIGNAL(currentIndexChanged(QString)), screenRecorder, SLOT(SetScreenshotsPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxUpdates, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetUpdatesPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxSecondsInCombat, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetSecondsInCombat(QString)));
    QObject::connect(uiConfig->comboBoxConsideredLines, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetConsideredLineCount(QString)));
    QObject::connect(uiConfig->pushButtonReset, SIGNAL(clicked(bool)), &m_Configurator, SLOT(RestoreDefaults()));
    QObject::connect(uiConfig->checkBoxProfColors, SIGNAL(clicked(bool)), this, SLOT(ProfSettingsChanged()));
    QObject::connect(uiConfig->checkBoxDamageDone, SIGNAL(clicked(bool)), this, SLOT(DamageDoneChanged()));
    QObject::connect(uiConfig->checkBoxPosition, SIGNAL(clicked(bool)), this, SLOT(PositionChanged()));
    QObject::connect(uiConfig->professionComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ProfChanged(QString)));
    QObject::connect(ui->pushButton, SIGNAL(toggled(bool)), this, SLOT(on_pushButton_toggled(bool)));

    dmgMeter->SetUpdatesPerSecond(uiConfig->comboBoxUpdates->currentText());
    dmgMeter->SetSecondsInCombat(uiConfig->comboBoxSecondsInCombat->currentText());
    dmgMeter->SetConsideredLineCount(uiConfig->comboBoxConsideredLines->currentText());

    m_ScreenRecorderThread.start();

    //Settings::ReadSettings<QMainWindow>(this);

    // Start screenshot timer from separate thread
    const int oldIndex = uiConfig->comboBoxScreenshots->currentIndex();
    uiConfig->comboBoxScreenshots->setCurrentIndex((uiConfig->comboBoxScreenshots->currentIndex() + 1) % uiConfig->comboBoxScreenshots->count());
    uiConfig->comboBoxScreenshots->setCurrentIndex(oldIndex);

    // Profession Color Bars
    ProfBasedColors=uiConfig->checkBoxProfColors->isChecked();
    pPosition=uiConfig->checkBoxPosition->isChecked();
    pDamageDone=uiConfig->checkBoxDamageDone->isChecked();
    uiConfig->professionComboBox->setCurrentIndex(0);
    m_MyProfession=uiConfig->professionComboBox->currentIndex();

    // We are not connected on start up
    is_connected = false;


    //New ProgressbarLabels
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

    // adding colomns to bars (not yet shown)
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

    labelname_0->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labeldmg_0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelper_0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labeldps_0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    labelact_0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // nameLabel->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(204,99,66, 70%);}");


    CheckFirstRun();
    CheckForUpdate();
    Initialize();
}

void MainWindow::ProfChanged(QString prof)
{
    if (prof== "Elementalist") m_MyProfession=1;else
        if (prof== "Engineer") m_MyProfession=2;else
            if (prof== "Guardian") m_MyProfession=3;else
                if (prof== "Mesmer") m_MyProfession=4;else
                    if (prof== "Necromancer") m_MyProfession=5;else
                        if (prof== "Ranger") m_MyProfession=6;else
                            if (prof== "Revenant") m_MyProfession=7;else
                                if (prof== "Thief") m_MyProfession=8;else
                                    if (prof== "Warrior") m_MyProfession=9;

// this should be more ressource efficent, but can't test it right now, esp. if prof==War
// https://stackoverflow.com/questions/767821/is-else-if-faster-than-switch-case
//    switch (prof) {
//    case "Elementalist": m_MyProfession=1; break;
//    case "Engineer"    : m_MyProfession=2; break;
//    case "Guardian"    : m_MyProfession=3; break;
//    case "Mesmer"      : m_MyProfession=4; break;
//    case "Necromancer" : m_MyProfession=5; break;
//    case "Ranger"      : m_MyProfession=6; break;
//    case "Revenant"    : m_MyProfession=7; break;
//    case "Thief"       : m_MyProfession=8; break;
//    case "Warrior"     : m_MyProfession=9; break;
//    }
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

void MainWindow::ProfSettingsChanged()
{
    if (ProfBasedColors==1) ProfBasedColors=0; else ProfBasedColors=1;
}
void MainWindow::PositionChanged()
{
    if (pPosition==1) pPosition=0; else pPosition=1;
}
void MainWindow::DamageDoneChanged()
{
    if (pDamageDone==1) pDamageDone=0; else pDamageDone=1;
}

void MainWindow::UpdateGroupLabels()
{
    QLabel* Label1;             // fuck you, dude!
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
        Bar[0]->setValue(i);

        // change this @syph
        QString text = QString("%L2 DMG - [%L3 DPS]").arg(PosDmg[0]).arg(PosDPS[0]);

        Bar[0]->setFormat(text);
        Bar[0]->setVisible(true);
        // Syph: I think this is useless, if not, please explain and change it back
        //ui->grp_Dmg->setText(QString::number(m_Dmg));
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

        // resting empty/disconnected slot to 0
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
            if (PosName[n][0]!=0) {
                if (PosDmg[0]>0)
                    i=PosDmg[n]*100/PosDmg[0];
                else i=0;
                if (GrpDmg>0)
                    p=PosDmg[n]*100/GrpDmg;
                else p=0;
//                Bar[n]->setValue(i);
//                //QString text = QString("%1. %2 %L3% [%L4 DPS]").arg(n+1).arg(PosName[n]).arg(p).arg(PosDPS[n]);
//                QString text;
//                if (pPosition>0)
//                {
//                    if (pDamageDone>0) text = QString("%1. %2 %L3% [%L4] [%L5 DPS]").arg(n+1).arg(PosName[n]).arg(p).arg(PosDmg[n]).arg(PosDPS[n]);
//                    else text = QString("%1. %2 %L3% [%L4 DPS]").arg(n+1).arg(PosName[n]).arg(p).arg(PosDPS[n]);
//                    Bar[n]->setFormat(text);
//                }
//                if (pPosition<1)
//                {
//                    if (pDamageDone<1) text = QString("%1 %L2% [%L3 DPS]").arg(PosName[n]).arg(p).arg(PosDPS[n]);
//                    else text = QString("%1 %L2% [%L3] [%L4 DPS]").arg(PosName[n]).arg(p).arg(PosDmg[n]).arg(PosDPS[n]);
//                    Bar[n]->setFormat(text);
//                }

//                Bar[n]->setAlignment(Qt::AlignRight);
                Bar[n]->setVisible(true);
//                if (ProfBasedColors>0)
//                {
//                    //profession based bar colors
//                    switch (PosProf[n])
//                    {
//                    case 0:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(3, 132, 146 , 60%);}");
//                        break;
//                    case 1:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(236, 87, 82, 70%);}");
//                        break;
//                    case 2:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(153,102,51, 70%);}");
//                        break;
//                    case 3:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(51,153,204, 70%);}");
//                        break;
//                    case 4:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(153,51,153, 70%);}");
//                        break;
//                    case 5:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(51,153,102, 70%);}");
//                        break;
//                    case 6:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(102,204,51, 70%);}");
//                        break;
//                    case 7:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(204,99,66, 70%);}");
//                        break;
//                    case 8:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(204,102,102, 70%);}");
//                        break;
//                    case 9:
//                        Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(255,153,51, 70%);}");
//                        break;
//                    }
//                }
//                else
//                {
//                    if (n%2==0 ) Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(3, 132, 146 , 60%);}");
//                    else Bar[n]->setStyleSheet("QProgressBar {border: 0px solid grey;border-radius:0px;font: 87 10pt DINPro-Black;color: rgb(255, 255, 255);text-align: center;min-height: 15px;margin: 0.5px;}QProgressBar::chunk {background-color: rgba(4,165,183, 60%);}");
//                }

                //                //Disable normal Text
                //Bar[n]->setTextVisible(false);

                QString myname = QString("%1. %2").arg(n+1).arg(PosName[n]);
                //                // DPS and %
                QString myDmg = QString("%L1").arg(PosDmg[n]);
                QString myPer = QString("%L1%").arg(p);
                //                //Set Text
                QString myDps = QString("[%L1 DPS]").arg(PosDPS[n]);


                labelname_0->setText(myname);
                labelname_1->setText(myname);

                labeldmg_0->setText(myDmg);
                labeldmg_1->setText(myDmg);

                labelper_0->setText(myPer);
                labelper_1->setText(myPer);

                labeldps_0->setText(myDps);
                labeldps_1->setText(myDps);

                //labelact_0->setText(myAct);
                //labelact_1->setText(myAct);

                labelname_1->setText(myname);
                labeldmg_1->setText(myDmg);

                labelname_2->setText(myname);
                labeldmg_2->setText(myDmg);
                //                //Styling them

                labelname_0->setStyleSheet("color:white;background:none;");
                labeldmg_0->setStyleSheet("color:white;background:none;");
                labelper_0->setStyleSheet("color:white;background:none;");
                labeldps_0->setStyleSheet("color:white;background:none;");

                labelname_1->setStyleSheet("color:white;background:none;text-align:center;");
                labeldmg_1->setStyleSheet("color:white;background:none;text-align:center;");

                labelname_2->setStyleSheet("color:white;background:none;text-align:center;");
                labeldmg_2->setStyleSheet("color:white;background:none;text-align:center;");
                //                //Align Them




                //                //Display Labels
                labelname_0->show();
                labeldmg_0->show();
                labelper_0->show();
                labeldps_0->show();
                //labelact_0->show();

                labelname_1->show();
                labeldmg_1->show();

                labelname_2->show();
                labeldmg_2->show();




            }
            else
                Bar[n] ->setVisible(false);
        }
    }
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
        this->show();
    }
    else
    {
        this->ui->centralWidget->setStyleSheet("background-color: rgba(32, 43, 47, 60%);");
        ui->toolBar->setStyleSheet("QWidget { background-color: rgba(32, 43, 47, 60%); } QToolButton { background-color: rgba(32, 43, 47, 1%); }");
        ui->grp_DPS->setStyleSheet("");
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
    Label1 = ui->critChance;
    Label1->setText(QString::number(m_critChance));
    //Personal Condi DMG Value
    Label1 =ui->labelCondiDMGValue;
    Label1->setText(QString::number(m_condiDmg));
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
    if ((is_connected == true))
    {
        ui->actionConnect->setIcon(QIcon(":/connected"));
        SendClientInfo();
    }
    else ui->actionConnect->setIcon(QIcon(":/connect"));
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
        hitCounter=0;
        m_critChance=0;
        critCounter=0;
        m_condiDmg=0;
        LastColor=0;

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
bool GW2::MainWindow::on_pushButton_toggled(bool toggeled)
{
    if (toggeled)
    {
        ui->widgetExtraDetails->show();
        toggeled = true;
    }
    else
    {
        ui->widgetExtraDetails->hide();
        toggeled = false;
    }
    return toggeled;
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
bool GW2::MainWindow::on_actionActionGroupDetails_toggled(bool toggeled)
{
    // Check if this user is playing on a server or not
    if ((is_connected == true))
    {
        if (toggeled)
        {
            ui->avg_DPS->show();
            ui->grp_DPS->show();
            ui->grp_Dmg->show();
            ui->labelDmg_2->show();
            ui->labelDmg_3->show();
            ui->labelDmg_3->setText("Grp DPS");
            ui->labelDmg_4->show();
            MainWindow::ui->widget->show();
            toggeled = true;
        }
        else
        {
            ui->widget->hide();
            toggeled = false;
        }
    }
    // If playing solo - show only DPS/DMG/TIME
    else
    {
        if (toggeled)
        {
            ui->avg_DPS->hide();
            ui->grp_DPS->hide();
            ui->labelDmg_3->hide();
            ui->labelDmg_4->hide();
            ui->grp_Dmg->hide();
            ui->labelDmg_2->hide();
            MainWindow::ui->widget->show();
            toggeled = true;
        }
        else
        {
            ui->widget->hide();
            toggeled = false;
        }
    }
    return toggeled;
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
        StartupPref();
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
            connect(download,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
            connect(changelog,SIGNAL(clicked(bool)),this,SLOT(on_pushButton2_clicked()));

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

void GW2::MainWindow::on_pushButton_clicked(){
    QString downloadlink = "http://gw2dps.com/download";
    QDesktopServices::openUrl(QUrl(downloadlink));
}

void GW2::MainWindow::on_pushButton2_clicked(){
    QString changeloglink = "http://gw2dps.com/changelog";
    QDesktopServices::openUrl(QUrl(changeloglink));
}
