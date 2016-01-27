#include "mytcpsocket.h"
#include "mydialog.h"
#include "global.h"

MyTcpSocket::MyTcpSocket(QObject *parent) :
    QObject(parent)
{
}

void MyTcpSocket::doConnect()
{
    socket = new QTcpSocket(this);


    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call

    // IP should be yours
    socket->connectToHost(myIP, 1234);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    //socket->write("1");
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
}

void MyTcpSocket::sendDmg(char* dmgME)
{
    //MyDialog dial;
    //QString syphdias = dial.getName();
    //qDebug() << "YOUR NAME: " << syphdias;
    qDebug()<< myVar;

    socket->write(myVar.toLatin1());
    socket->write(": ");
    socket->write(dmgME);
}

