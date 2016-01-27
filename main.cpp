#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QtNetwork>
#include "dmgmeter.h"
#include "dmgmeter.cpp"
#include <sstream>
#include "global.h"
#include "mytcpsocket.h"
#include "mydialog.h"


using namespace std;

QString myVar;
QString myIP;
int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    qDebug() << "main:" << "thread id" << QThread::currentThreadId();
#endif // QT_DEBUG
    QApplication a(argc, argv);

    MyDialog mDialog;
    mDialog.setModal(true);
    mDialog.exec();

    QString DMGNAME = mDialog.getName();
    QString MYSEXYIP = mDialog.getIP();
    //QString myVar = DMGNAME;
    myVar = DMGNAME;
    myIP = MYSEXYIP;

    qDebug() << DMGNAME;
    qDebug() << MYSEXYIP;


    GW2::MainWindow w;
    w.show();

    //qDebug() << "hello: " << SocketDescriptor;
    return a.exec();
}


