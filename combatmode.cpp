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
    accept();
}

// Give movement access to CombatMode
void CombatMode::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// Give movement access to CombatMode
void CombatMode::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void CombatMode::reject()
{
    QDialog::reject();
}
