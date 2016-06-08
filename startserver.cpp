#include "startserver.h"
#include "ui_startserver.h"
#include <QDebug>
#include <global.h>
#include <QProcessEnvironment>
#include <QDir>
#include <settings.h>

using namespace GW2;

startserver::startserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startserver)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);

    Settings::ReadSettings(ui->lineEditPort);
    Settings::ReadSettings(ui->lineEditAuth);

    QRegExp portauthRx("^\\d{0,5}");
    QValidator *portauthVal = new QRegExpValidator(portauthRx, this);
    ui->lineEditPort->setValidator(portauthVal);
    ui->lineEditAuth->setValidator(portauthVal);
}

startserver::~startserver()
{
    Settings::WriteSettings(ui->lineEditPort);
    Settings::WriteSettings(ui->lineEditAuth);

    delete ui;
}

void GW2::startserver::on_pushButton_clicked()
{
    QString portvalue = ui->lineEditPort->text();
    QString authvalue = ui->lineEditAuth->text();

    #ifdef Q_OS_WIN
    QString command = "gw2specs_server.exe";
    #elif defined(Q_OS_LINUX)
    QString command = "gw2specs_server";
    #elif defined(Q_OS_MACX)
    QString command = "gw2specs_server.dmg"
    #else
        qDebug()<< "You are using a weird OS";
    #error "Something went completely wrong. Please contact Tyrox.1352"
    #endif
    QStringList parameters;
    parameters << "-p" << portvalue << "-a" << authvalue;
    QProcess *server = new QProcess(this);
    server->startDetached(command, parameters);
    close();
}


