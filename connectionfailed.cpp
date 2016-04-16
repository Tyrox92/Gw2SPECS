#include "connectionfailed.h"
#include "ui_connectionfailed.h"
#include "mainwindow.h"

using namespace GW2;

connectionfailed::connectionfailed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectionfailed)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
}

connectionfailed::~connectionfailed()
{
    delete ui;
}
