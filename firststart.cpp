#include "firststart.h"
#include "ui_firststart.h"
#include "mainwindow.h"

using namespace GW2;

firstStart::firstStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstStart)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
}

firstStart::~firstStart()
{
    delete ui;
}
