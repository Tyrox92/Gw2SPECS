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

using namespace GW2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Configurator(this),
    update_Timer(this)
{
    QObject::connect(&update_Timer, SIGNAL(timeout()), this, SLOT(UpdateTimer()));
    ui->setupUi(this);
    StartupHideProgressBars();
    ui->widgetExtraDetails->hide();
    ui->toolBar->setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBar->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // Resize Option
    // QGridLayout is already in *.ui file
    // Using gridLayout_3 here which is the outer layout
    QSizeGrip *sizeGripRight = new QSizeGrip(this);
    QSizeGrip *sizeGripLeft = new QSizeGrip(this);
    ui->gridLayout_Main->addWidget(sizeGripRight, 0,0,10,10,Qt::AlignBottom | Qt::AlignRight);
    ui->gridLayout_Main->addWidget(sizeGripLeft, 0,0,10,10,Qt::AlignBottom | Qt::AlignLeft);
    sizeGripLeft->setStyleSheet("background: url(''); width: 20px; height: 20px;");
    sizeGripRight->setStyleSheet("background: url(''); width: 20px; height: 20px;");


    //QObject::connect(ui->btnTransparency, SIGNAL(clicked(bool)), this, SLOT(EnableTransparency(bool)));
    //QObject::connect(ui->btnHelp, SIGNAL(clicked()), this, SLOT(LinkToWebsite()));
    //QObject::connect(ui->btnConfig, SIGNAL(clicked()), &m_Configurator, SLOT(exec()));
    QObject::connect(ui->actionEnableTransparency, SIGNAL(triggered(bool)), this, SLOT(EnableTransparency(bool)));
    QObject::connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(LinkToWebsite()));
    QObject::connect(ui->actionConfig, SIGNAL(triggered()), &m_Configurator, SLOT(exec()));

    ScreenRecorder* screenRecorder = new ScreenRecorder;
    DmgMeter* dmgMeter = &screenRecorder->GetDmgMeter();
    screenRecorder->moveToThread(&m_ScreenRecorderThread);

    Ui::Configurator* uiConfig = m_Configurator.ui;
    dmgMeter->moveToThread(&m_ScreenRecorderThread);

    QObject::connect(&m_ScreenRecorderThread, SIGNAL(finished()), screenRecorder, SLOT(deleteLater()));
    QObject::connect(&m_ScreenRecorderThread, SIGNAL(started()), screenRecorder, SLOT(StartRecording()));
    QObject::connect(screenRecorder, SIGNAL(RequestInfoUpdate(QString)), ui->labelInfo, SLOT(setText(QString)));
    QObject::connect(dmgMeter, SIGNAL(RequestTimeUpdate(int)), this, SLOT(UpdateTime(int)));
    //QObject::connect(dmgMeter, SIGNAL(RequestDmgUpdate(unsigned long long)), this, SLOT(UpdateDmg(unsigned long long)));
    //QObject::connect(dmgMeter, SIGNAL(RequestDpsUpdate(int)), this, SLOT(UpdatePersonal()));
    //QObject::connect(dmgMeter, SIGNAL(RequestMaxDmgUpdate(int)), this, SLOT(UpdateMaxDmg(int)));
    //QObject::connect(ui->btnReset, SIGNAL(clicked()), dmgMeter, SLOT(Reset()));
    //QObject::connect(ui->btnAutoReset, SIGNAL(triggered(bool)), dmgMeter, SLOT(SetIsAutoResetting(bool)));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), dmgMeter, SLOT(Reset()));
    QObject::connect(ui->actionAutoReset, SIGNAL(triggered(bool)), dmgMeter, SLOT(SetIsAutoResetting(bool)));
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
    m_critChance=0;
    critCounter=0;
    m_condiDmg=0;
    LastColor=0;

    socket->connectToHost(HostIP, HostPort);

    // we need to wait...
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
        dialog->show();
    }
    m_Dps=0;m_Dmg=0;m_Activity=0;m_MaxDmg=0;
    update_Timer.start(1000);
}

void MainWindow::UpdateGroupLabels()
{
    QLabel* Label1;
    QProgressBar* Bar1;
    long p,i,j,k;

    if (MyClientSlot!=10)  //connected and semi-handshaked
    {
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
            for (int p=0;p<9;p++)
            {
                if (PosName[p]==QString("Disconnected"))
                {
                    PosName[p][0]=0;
                    PosDmg[p]=0;
                    PosDPS[p]=0;
                    PosAct[p]=0;
                }
            }
        }

        for(int n=0;n<10;n++) {
            if (PosName[n][0]!=0) {
                if (PosDmg[0]>0)i=PosDmg[n]*100/PosDmg[0];
                else i=0;
                if (AllDamageDone>0)p=PosDmg[n]*100/AllDamageDone;
                else p=0;
                switch (n) {
                case 0: Bar1 = ui->progressBar_1; break;
                case 1: Bar1 = ui->progressBar_2; break;
                case 2: Bar1 = ui->progressBar_3; break;
                case 3: Bar1 = ui->progressBar_4; break;
                case 4: Bar1 = ui->progressBar_5; break;
                case 5: Bar1 = ui->progressBar_6; break;
                case 6: Bar1 = ui->progressBar_7; break;
                case 7: Bar1 = ui->progressBar_8; break;
                case 8: Bar1 = ui->progressBar_9; break;
                case 9: Bar1 = ui->progressBar_10; break;
                default: break;
                }
                Bar1->setValue(i);
                QString text = QString("%1. %2     %L3 (%L4 | %5%)").arg(n+1).arg(PosName[n]).arg(PosDmg[n]).arg(PosDPS[n]).arg(p);
                Bar1->setFormat(text);
                Bar1->setVisible(true);
            }
            else switch (n) {
            case 0: ui->progressBar_1 ->setVisible(false); break;
            case 1: ui->progressBar_2 ->setVisible(false); break;
            case 2: ui->progressBar_3 ->setVisible(false); break;
            case 3: ui->progressBar_4 ->setVisible(false); break;
            case 4: ui->progressBar_5 ->setVisible(false); break;
            case 5: ui->progressBar_6 ->setVisible(false); break;
            case 6: ui->progressBar_7 ->setVisible(false); break;
            case 7: ui->progressBar_8 ->setVisible(false); break;
            case 8: ui->progressBar_9 ->setVisible(false); break;
            case 9: ui->progressBar_10 ->setVisible(false); break;
            default: break;
            }
        }

        /*
        if (PosName[0][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[0]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[0]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_1;
            Bar1->setValue(i);
            QString text = QString("1. %1     %L2 (%L3 | %4%)").arg(PosName[0]).arg(PosDmg[0]).arg(PosDPS[0]).arg(p);
            Bar1->setFormat(text);
            Bar1->setVisible(true);

        }
        else ui->progressBar_1->setVisible(false);

        if (PosName[1][0]!=0)
        {
            if (PosDmg[0]>0)i=PosDmg[1]*100/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[1]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_2;
            Bar1->setValue(i);
            QString text = QString("2. %1     %L2 (%L3 | %4%)").arg(PosName[1]).arg(PosDmg[1]).arg(PosDPS[1]).arg(p);
            Bar1->setFormat(text);
            Bar1->setVisible(true);

        }
        else ui->progressBar_2->setVisible(false);

        if (PosName[2][0]!=0)
        {
            if (PosDmg[0]>0)i=PosDmg[2]*100/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[2]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_3;
            QString text = QString("3. %1     %L2 (%L3 | %4%)").arg(PosName[2]).arg(PosDmg[2]).arg(PosDPS[2]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_3->setVisible(false);

        if (PosName[3][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[3]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[3]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_4;
            QString text = QString("4. %1     %L2 (%L3 | %4%)").arg(PosName[3]).arg(PosDmg[3]).arg(PosDPS[3]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_4->setVisible(false);

        if (PosName[4][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[4]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[4]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_5;
            QString text = QString("5. %1     %L2 (%L3 | %4%)").arg(PosName[4]).arg(PosDmg[4]).arg(PosDPS[4]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_5->setVisible(false);


        if (PosName[5][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[5]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[5]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_6;
            QString text = QString("6. %1     %L2 (%L3 | %4%)").arg(PosName[5]).arg(PosDmg[5]).arg(PosDPS[5]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_6->setVisible(false);

        if (PosName[6][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[6]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[6]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_7;
            QString text = QString("7. %1     %L2 (%L3 | %4%)").arg(PosName[6]).arg(PosDmg[6]).arg(PosDPS[6]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_7->setVisible(false);

        if (PosName[7][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[7]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[7]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_8;
            QString text = QString("8. %1     %L2 (%L3 | %4%)").arg(PosName[7]).arg(PosDmg[7]).arg(PosDPS[7]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);
        }
        else ui->progressBar_8->setVisible(false);

        if (PosName[8][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[8]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[8]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_9;
            QString text = QString("9. %1     %L2 (%L3 | %4%)").arg(PosName[8]).arg(PosDmg[8]).arg(PosDPS[8]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);

        }
        else ui->progressBar_9->setVisible(false);

        if (PosName[9][0]!=0)
        {
            if (PosDmg[0]>0 )i=PosDmg[9]*100.0/PosDmg[0];else i=0;
            if (AllDamageDone>0)p=PosDmg[9]*100/AllDamageDone;else p=0;
            Bar1 = ui->progressBar_10;
            QString text = QString("10. %1     %L2 (%L3 | %4%)").arg(PosName[9]).arg(PosDmg[9]).arg(PosDPS[9]).arg(p);
            Bar1->setValue(i);
            Bar1->setFormat(text);
            Bar1->setVisible(true);

        }
        else ui->progressBar_10->setVisible(false);
*/
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

                if ((incData2[i+1]<58) && (incData2[i+1]>47) && (incData2[i+2]>48) && (incData2[i+2]<53))
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
    QString styleTransparent = " QGroupBox#groupBoxSettings {\
            background-color: rgb(248,248,248);\
            border: 1px solid rgb(170, 170, 255);\
            border-radius: 3px;\
            border-color:rgb(170, 170, 255);\
    }";

    if (isAlmostTransparent)
    {
        this->ui->widget_4->setStyleSheet("background-color: rgba(32, 43, 47, 0%);");
        ui->toolBar->setStyleSheet("QWidget { background-color: rgba(32, 43, 47, 1%); } QToolButton { background-color: rgba(32, 43, 47, 1%); }");
        ui->labelDpsValue->setStyleSheet("");
        this->show();

    }
    else
    {
        this->ui->widget_4->setStyleSheet("background-color: rgba(32, 43, 47, 60%);");
        ui->toolBar->setStyleSheet("QWidget { background-color: rgba(32, 43, 47, 60%); } QToolButton { background-color: rgba(32, 43, 47, 1%); }");
        ui->labelDpsValue->setStyleSheet("");
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

        sprintf(writeBuff, "*%u1#%s*%u2#%lu*%u3#%lu*%u4#%lu*", MyClientSlot, tmp2 , MyClientSlot, m_Dps, MyClientSlot, m_Dmg, MyClientSlot, m_Activity);

        socket->write(writeBuff);
    }
}

void MainWindow::UpdatePersonalLabels()
{
    QLabel* Label1;
    long i;

    //Personal DPS Value
    Label1 = ui->labelDpsValue;
    Label1->setText(QString::number(m_Dps));
    //Personal Crit Chance Value
    Label1 = ui->critChance;
    Label1->setText(QString::number(m_critChance));
    //Personal Condi DPS Value
    if (m_Dmg>0) i=m_condiDmg*m_Dps/m_Dmg; else i=0;
    Label1 = ui->condiDPS;
    Label1->setText(QString::number(i));
    //Personal Max Damage Value
    Label1 = ui->labelMaxDmgValue;
    Label1->setText(QString::number(m_MaxDmg));
    //Personal Damage Value
    Label1 =ui->labelDmgValue;
    Label1->setText(QString::number(m_Dmg));
    //Personal Condi DMG Value
    Label1 =ui->labelCondiDMGValue;
    Label1->setText(QString::number(m_condiDmg));


    //dps and max dmg limits style, we can keep them if you want
    //limit consts are in the dmgmeter.h
    //style const are in dmgmeter.cpp

    Label1 = ui->labelDpsValue;
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

    Label1 = ui->labelMaxDmgValue;
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
    SendClientInfo();
    UpdatePersonalLabels();
    UpdateGroupLabels();
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

void GW2::MainWindow::on_pushButton_toggled(bool checked)
{
    QSettings settings;
    if (checked)
    {
        ui->widgetExtraDetails->show();
    }
    else
    {
        ui->widgetExtraDetails->hide();
    }
}

void GW2::MainWindow::on_toolBar_dockLocationChange()
{

}

void GW2::MainWindow::on_btn_grpMoreInfo_clicked()
{
}

void GW2::MainWindow::StartupHideProgressBars()
{
    ui->progressBar_1->setVisible(false);
    ui->progressBar_2->setVisible(false);
    ui->progressBar_3->setVisible(false);
    ui->progressBar_4->setVisible(false);
    ui->progressBar_5->setVisible(false);
    ui->progressBar_6->setVisible(false);
    ui->progressBar_7->setVisible(false);
    ui->progressBar_8->setVisible(false);
    ui->progressBar_9->setVisible(false);
    ui->progressBar_10->setVisible(false);
}
