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

// Give movement access to SaveLog
void saveLog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

// Give movement access to SaveLog
void saveLog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void GW2::saveLog::on_saveTXT_clicked()
{
    this->close();
}

void GW2::saveLog::on_saveCSV_clicked()
{
    this->close();
}

void GW2::saveLog::on_saveALL_clicked()
{
    this->close();
}
