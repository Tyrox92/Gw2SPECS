#include "savelog.h"
#include "ui_savelog.h"
#include "mainwindow.h"

using namespace GW2;

saveLog::saveLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveLog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
}

saveLog::~saveLog()
{
    delete ui;
}
