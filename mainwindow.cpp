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

using namespace GW2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Configurator(this),
    update_Timer(this)
{
    QObject::connect(&update_Timer, SIGNAL(timeout()), this, SLOT(UpdateLabels()));
    ui->setupUi(this);
    //ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //Resize Option
    // QGridLayout is already in *.ui file
    // Using gridLayout here which is the outer layout
    QSizeGrip * sizeGrip = new QSizeGrip(this);
    ui->gridLayout->addWidget(sizeGrip, 0,0,10,10,Qt::AlignBottom | Qt::AlignRight);
    sizeGrip->setStyleSheet("background: url(''); width: 16px; height: 16px;");

    QObject::connect(ui->btnTransparency, SIGNAL(clicked(bool)), this, SLOT(EnableTransparency(bool)));
    QObject::connect(ui->btnHelp, SIGNAL(clicked()), this, SLOT(LinkToWebsite()));
    QObject::connect(ui->btnConfig, SIGNAL(clicked()), &m_Configurator, SLOT(exec()));

    ScreenRecorder* screenRecorder = new ScreenRecorder;
    DmgMeter* dmgMeter = &screenRecorder->GetDmgMeter();
    screenRecorder->moveToThread(&m_ScreenRecorderThread);

    Ui::Configurator* uiConfig = m_Configurator.ui;
    dmgMeter->moveToThread(&m_ScreenRecorderThread);

    QObject::connect(&m_ScreenRecorderThread, SIGNAL(finished()), screenRecorder, SLOT(deleteLater()));
    QObject::connect(&m_ScreenRecorderThread, SIGNAL(started()), screenRecorder, SLOT(StartRecording()));
    QObject::connect(screenRecorder, SIGNAL(RequestInfoUpdate(QString)), ui->labelInfo, SLOT(setText(QString)));
    QObject::connect(dmgMeter, SIGNAL(RequestTimeUpdate(int)), this, SLOT(UpdateTime(int)));
    QObject::connect(dmgMeter, SIGNAL(RequestDmgUpdate(unsigned long long)), this, SLOT(UpdateDmg(unsigned long long)));
    QObject::connect(dmgMeter, SIGNAL(RequestDpsUpdate(int)), this, SLOT(UpdateDps(int)));
    QObject::connect(dmgMeter, SIGNAL(RequestMaxDmgUpdate(int)), this, SLOT(UpdateMaxDmg(int)));
    QObject::connect(ui->btnReset, SIGNAL(clicked()), dmgMeter, SLOT(Reset()));
    QObject::connect(ui->btnAutoReset, SIGNAL(triggered(bool)), dmgMeter, SLOT(SetIsAutoResetting(bool)));
    QObject::connect(uiConfig->comboBoxScreenshots, SIGNAL(currentIndexChanged(QString)), screenRecorder, SLOT(SetScreenshotsPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxUpdates, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetUpdatesPerSecond(QString)));
    QObject::connect(uiConfig->comboBoxSecondsInCombat, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetSecondsInCombat(QString)));
    QObject::connect(uiConfig->comboBoxConsideredLines, SIGNAL(currentIndexChanged(QString)), dmgMeter, SLOT(SetConsideredLineCount(QString)));
    QObject::connect(uiConfig->pushButtonReset, SIGNAL(clicked(bool)), &m_Configurator, SLOT(RestoreDefaults()));

    dmgMeter->SetUpdatesPerSecond(uiConfig->comboBoxUpdates->currentText());
    dmgMeter->SetSecondsInCombat(uiConfig->comboBoxSecondsInCombat->currentText());
    dmgMeter->SetConsideredLineCount(uiConfig->comboBoxConsideredLines->currentText());

    m_ScreenRecorderThread.start();

    Settings::ReadSettings<QMainWindow>(this);

    // Start screenshot timer from separate thread
    const int oldIndex = uiConfig->comboBoxScreenshots->currentIndex();
    uiConfig->comboBoxScreenshots->setCurrentIndex((uiConfig->comboBoxScreenshots->currentIndex() + 1) % uiConfig->comboBoxScreenshots->count());
    uiConfig->comboBoxScreenshots->setCurrentIndex(oldIndex);

    socket = new QTcpSocket(this);


    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(ready2Read()));

    connect(dmgMeter, SIGNAL(RequestNetWrite(char*)), this, SLOT(netWrite(char*)));

    qDebug() << "connecting to : " << HostIP << ":" << HostPort;

    // this is not blocking call

    // IP should be yours
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
    AllDamageDone=0;
    hitCounter=0;
    critChance=0;
    critCounter=0;
    condiDmg=0;
    LastColor=0;

    socket->connectToHost(HostIP, HostPort);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
    m_Dps=0;m_Dmg=0;m_Activity=0;
    update_Timer.start(1000);
}

void MainWindow::UpdateLabels()
{
    QLabel* Label1;
    QProgressBar* Bar1;
    long i,j,k;

    tmp3 = MyName.toLatin1();
    tmp4 = tmp3.data();
    if (MyClientSlot!=10)  //connected and semi-handshaked
    {
        sprintf(writeBuff, "*%u1#%s*%u2#%u*%u3#%u*%u4#%u*", MyClientSlot, tmp4 , MyClientSlot, m_Dps, MyClientSlot, m_Dmg, MyClientSlot, m_Activity);

        socket->write(writeBuff);
        Label1 = ui->critChance;
        Label1->setText(QString::number(critChance));
        if (m_Dmg>0) i=condiDmg*m_Dps/m_Dmg; else i=0;
        Label1 = ui->condiDPS;
        Label1->setText(QString::number(i));
        AllDamageDone=0;
        for (j=0;j<10;j++) AllDamageDone+=SlotDmg[j];
        Label1 = ui->grp_Dmg;
        Label1->setText(QString::number(AllDamageDone));
        GrpDPS=0;
        for (j=0;j<10;j++) GrpDPS+=SlotDPS[j];
        Label1 = ui->grp_DPS;
        Label1->setText(QString::number(GrpDPS));
        i=0;
        for (j=0;j<10;j++) if (SlotDPS[j]>0) i++;
        if (i>0) AvgDPS=GrpDPS/i; else AvgDPS=0;
        Label1 = ui->avg_DPS;
        Label1->setText(QString::number(AvgDPS));

        for (j=0;j<10;j++)
        {
            PosDmg[j]=SlotDmg[j];
            PosDPS[j]=SlotDPS[j];
            PosAct[j]=SlotAct[j];
            strcpy(PosName[j],SlotName[j]);
        }

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
                    k=PosAct[i];
                    PosAct[i]=PosAct[j];
                    PosAct[j]=k;
                    strcpy(tmp1,PosName[i]);
                    strcpy(PosName[i],PosName[j]);
                    strcpy(PosName[j],tmp1);
                }
            }
        }

        if (PosName[0][0]!=0)
        {
            Label1 = ui->Dmg_1;
            Label1->setText(QString::number(PosDmg[0]));
            Label1 = ui->Dps_1;
            Label1->setText(QString::number(PosDPS[0]));
            Label1 = ui->Act_1;
            Label1->setText(QString::number(PosAct[0]));
            Label1 = ui->Pos_1;
            Label1->setText(PosName[0]);
            if (AllDamageDone>0 )i=PosDmg[0]*100.0/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_1;
            Bar1->setValue(i);
        }

        if (PosName[1][0]!=0)
        {
            Label1 = ui->Dmg_2;
            Label1->setText(QString::number(PosDmg[1]));
            Label1 = ui->Dps_2;
            Label1->setText(QString::number(PosDPS[1]));
            Label1 = ui->Act_2;
            Label1->setText(QString::number(PosAct[1]));
            Label1 = ui->Pos_2;
            Label1->setText(PosName[1]);
            if (AllDamageDone>0 )i=PosDmg[1]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_2;
            Bar1->setValue(i);
        }
        if (PosName[2][0]!=0)
        {
            Label1 = ui->Dmg_3;
            Label1->setText(QString::number(PosDmg[2]));
            Label1 = ui->Dps_3;
            Label1->setText(QString::number(PosDPS[2]));
            Label1 = ui->Act_3;
            Label1->setText(QString::number(PosAct[2]));
            Label1 = ui->Pos_3;
            Label1->setText(PosName[2]);
            if (AllDamageDone>0 )i=PosDmg[2]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_3;
            Bar1->setValue(i);
        }
        if (PosName[3][0]!=0)
        {
            Label1 = ui->Dmg_4;
            Label1->setText(QString::number(PosDmg[3]));
            Label1 = ui->Dps_4;
            Label1->setText(QString::number(PosDPS[3]));
            Label1 = ui->Act_4;
            Label1->setText(QString::number(PosAct[3]));
            Label1 = ui->Pos_4;
            Label1->setText(PosName[3]);
            if (AllDamageDone>0 )i=PosDmg[3]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_4;
            Bar1->setValue(i);
        }
        if (PosName[4][0]!=0)
        {
            Label1 = ui->Dmg_5;
            Label1->setText(QString::number(PosDmg[4]));
            Label1 = ui->Dps_5;
            Label1->setText(QString::number(PosDPS[4]));
            Label1 = ui->Act_5;
            Label1->setText(QString::number(PosAct[4]));
            Label1 = ui->Pos_5;
            Label1->setText(PosName[4]);
            if (AllDamageDone>0 )i=PosDmg[4]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_5;
            Bar1->setValue(i);
        }

        if (PosName[5][0]!=0)
        {
            Label1 = ui->Dmg_6;
            Label1->setText(QString::number(PosDmg[5]));
            Label1 = ui->Dps_6;
            Label1->setText(QString::number(PosDPS[5]));
            Label1 = ui->Act_6;
            Label1->setText(QString::number(PosAct[5]));
            Label1 = ui->Pos_6;
            Label1->setText(PosName[5]);
            if (AllDamageDone>0 )i=PosDmg[5]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_6;
            Bar1->setValue(i);
        }

        if (PosName[6][0]!=0)
        {
            Label1 = ui->Dmg_7;
            Label1->setText(QString::number(PosDmg[6]));
            Label1 = ui->Dps_7;
            Label1->setText(QString::number(PosDPS[6]));
            Label1 = ui->Act_7;
            Label1->setText(QString::number(PosAct[6]));
            Label1 = ui->Pos_7;
            Label1->setText(PosName[6]);
            if (AllDamageDone>0 )i=PosDmg[6]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_7;
            Bar1->setValue(i);
        }

        if (PosName[7][0]!=0)
        {
            Label1 = ui->Dmg_8;
            Label1->setText(QString::number(PosDmg[7]));
            Label1 = ui->Dps_8;
            Label1->setText(QString::number(PosDPS[7]));
            Label1 = ui->Act_8;
            Label1->setText(QString::number(PosAct[7]));
            Label1 = ui->Pos_8;
            Label1->setText(PosName[7]);
            if (AllDamageDone>0 )i=PosDmg[7]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_8;
            Bar1->setValue(i);
        }

        if (PosName[8][0]!=0)
        {
            Label1 = ui->Dmg_9;
            Label1->setText(QString::number(PosDmg[8]));
            Label1 = ui->Dps_9;
            Label1->setText(QString::number(PosDPS[8]));
            Label1 = ui->Act_9;
            Label1->setText(QString::number(PosAct[8]));
            Label1 = ui->Pos_9;
            Label1->setText(PosName[8]);
            if (AllDamageDone>0 )i=PosDmg[8]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_9;
            Bar1->setValue(i);
        }

        if (PosName[9][0]!=0)
        {
            Label1 = ui->Dmg_10;
            Label1->setText(QString::number(PosDmg[9]));
            Label1 = ui->Dps_10;
            Label1->setText(QString::number(PosDPS[9]));
            Label1 = ui->Act_10;
            Label1->setText(QString::number(PosAct[9]));
            Label1 = ui->Pos_10;
            Label1->setText(PosName[9]);
            if (AllDamageDone>0 )i=PosDmg[9]*100/AllDamageDone;else i=0;
            Bar1 = ui->progressBar_10;
            Bar1->setValue(i);
        }
    }
}




void MainWindow::ready2Read()
{

    int c,i,j;
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

                 if ((incData2[i+1]<58) && (incData2[i+1]>47) && (incData2[i+2]>48) && (incData2[i+2]<53))
                     {
                        CurrentPos=incData2[i+1]-48;
                        CurrentMeta=incData2[i+2]-48;
                        i+=3;
                     }

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
                         while ((j-i-1<12) && (j<incDataSize) && (incData2[j]!='*')) { k=k*10+incData2[j]-48;j++; }
                         if  (incData2[j]=='*')
                          {

                             if  (CurrentMeta==2) SlotDPS[CurrentPos]=k;
                             if  (CurrentMeta==3) SlotDmg[CurrentPos]=k;
                             if  (CurrentMeta==4) SlotAct[CurrentPos]=k;
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
    qDebug() << "disconnected...";
    //so what now? exit?

}


void MainWindow::netWrite(char* c)
{
    socket->write(c);
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
        this->setStyleSheet("background-color: rgba(32, 43, 47, 1%);");
        this->show();

    }
    else
    {
        this->setStyleSheet("background-color: rgba(32, 43, 47, 50%);");
        this->show();
    }
}



void MainWindow::LinkToWebsite()
{
    //QDesktopServices::openUrl(QUrl(MAINWINDOW_WEBSITE_URL));
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

void MainWindow::UpdateDmg(unsigned long long dmg)
{
    ui->labelDmgValue->setText(QString::number(dmg));

}

void MainWindow::UpdateDps(int dps)
{
    QLabel* Label1;
    QProgressBar* Bar1;
    int i,j,k;


    QLabel* dpsLabel = ui->labelDpsValue;
    dpsLabel->setText(QString::number(dps));
    if (dps > DMGMETER_HIGH_DPS_LIMIT)
    {
        dpsLabel->setStyleSheet(DmgMeter::s_UltraStyle);
    }
    else if (dps > DMGMETER_NORMAL_DPS_LIMIT)
    {
        dpsLabel->setStyleSheet(DmgMeter::s_HighStyle);
    }
    else if (dps > DMGMETER_LOW_DPS_LIMIT)
    {
        dpsLabel->setStyleSheet(DmgMeter::s_NormalStyle);
    }
    else
    {
        dpsLabel->setStyleSheet(DmgMeter::s_LowStyle);
    }

}

void MainWindow::UpdateMaxDmg(int maxDmg)
{
    QLabel* maxDmgLabel = ui->labelMaxDmgValue;
    maxDmgLabel->setText(QString::number(maxDmg));
    if (maxDmg > DMGMETER_HIGH_MAX_DMG_LIMIT)
    {
        maxDmgLabel->setStyleSheet(DmgMeter::s_UltraStyle);
    }
    else if (maxDmg > DMGMETER_NORMAL_MAX_DMG_LIMIT)
    {
        maxDmgLabel->setStyleSheet(DmgMeter::s_HighStyle);
    }
    else if (maxDmg > DMGMETER_LOW_MAX_DMG_LIMIT)
    {
        maxDmgLabel->setStyleSheet(DmgMeter::s_NormalStyle);
    }
    else
    {
        maxDmgLabel->setStyleSheet(DmgMeter::s_LowStyle);
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


