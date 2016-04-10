#include "combatmode.h"
#include "ui_combatmode.h"
#include "mainwindow.h"
#include <QDebug>

using namespace GW2;

CombatMode::CombatMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombatMode)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
}

CombatMode::~CombatMode()
{
    delete ui;
}

void CombatMode::on_buttonResetCombatMode_clicked()
{
    qDebug() << "Debug working";
}
