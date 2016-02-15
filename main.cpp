#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QtNetwork>
#include "dmgmeter.h"
#include "dmgmeter.cpp"
#include <sstream>
#include "global.h"
#include "QHBoxLayout"
#include "settings.h"

using namespace GW2;

int MyClientSlot;
unsigned long m_Dps;
unsigned long m_Dmg;
unsigned long m_Activity;
int LastColor;
unsigned long hitCounter;
unsigned long critCounter;
unsigned int m_critChance;
unsigned long m_condiDmg;
unsigned long m_MaxDmg;
unsigned long m_MyProfession;
//QString curVersion = Settings::s_Version;

int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    qDebug() << "main:" << "thread id" << QThread::currentThreadId();
#endif // QT_DEBUG
    QApplication a(argc, argv);

    GW2::MainWindow w;
    w.show();

    return a.exec();
}


