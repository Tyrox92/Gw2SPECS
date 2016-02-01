#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QtNetwork>
#include "dmgmeter.h"
#include "dmgmeter.cpp"
#include <sstream>
#include "global.h"
#include "mydialog.h"

using namespace std;

QString MyName;
QString HostIP;
int MyClientSlot;
int HostPort;
int m_Dps;
unsigned long m_Dmg;
unsigned long m_Activity;
int LastColor;
long hitCounter;
long critCounter;
int critChance;
long condiDmg;



int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    qDebug() << "main:" << "thread id" << QThread::currentThreadId();
#endif // QT_DEBUG
    QApplication a(argc, argv);

    MyDialog mDialog;
    mDialog.setModal(true);


    mDialog.exec();

    MyName=mDialog.getName();
    HostIP = mDialog.getIP();
    HostPort=mDialog.getPort();

    GW2::MainWindow w;
    w.show();



    return a.exec();
}


