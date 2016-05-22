#include "startserver.h"
#include "ui_startserver.h"
#include <QDebug>
#include <global.h>

using namespace GW2;

startserver::startserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startserver)
{
    ui->setupUi(this);
}

startserver::~startserver()
{
    delete ui;
}
