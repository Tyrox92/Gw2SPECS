#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QtNetwork>
#include "dmgmeter.h"
#include "dmgmeter.cpp"
#include <sstream>
#include "global.h"
#include "QHBoxLayout"

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
QString curVersion = "2.3";

int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    qDebug() << "main:" << "thread id" << QThread::currentThreadId();
#endif // QT_DEBUG
    QApplication a(argc, argv);


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
        if(curVersion < ver){
            qDebug() << "You need to Update";
            QDialog *dialog = new QDialog();
            QHBoxLayout *layout = new QHBoxLayout(dialog);
            QLabel *label = new QLabel();
            label->setText("A newer Version of GW2DPS is available!<br><hr><br><a href='#'>Download new Version</a>");
            layout->addWidget(label);
            layout->setMargin(50);
            dialog->setStyleSheet("background:#f2f2f2;");
            dialog->show();
        }
    }



    GW2::MainWindow w;
    w.show();

    return a.exec();
}


