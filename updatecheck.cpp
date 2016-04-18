#include "updatecheck.h"
#include "ui_updatecheck.h"
#include "mainwindow.h"

using namespace GW2;

updateCheck::updateCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::updateCheck)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
}

updateCheck::~updateCheck()
{
    delete ui;
}


// Give movement access to UpdateCheck
void updateCheck::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// Give movement access to UpdateCheck
void updateCheck::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
