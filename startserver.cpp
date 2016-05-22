#include "startserver.h"
#include "ui_startserver.h"
#include <QDebug>

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

void GW2::startserver::on_keySequenceEdit_editingFinished()
{
    qDebug()<< "DONE CHANGING: " << ui->keySequenceEdit->keySequence().toString();
}

