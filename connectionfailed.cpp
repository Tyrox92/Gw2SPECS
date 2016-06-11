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

// Give movement access to ConnectionFailed
void connectionfailed::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// Give movement access to ConnectionFailed
void connectionfailed::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
