#ifndef MYDIALOG_CPP
#include "mydialog.h"
#include "ui_mydialog.h"
#include <QDebug>
#include <settings.h>
#include "mainwindow.h"

using namespace GW2;


MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    ui->lineEdit->setText(ReadNameSettings());
    ui->lineEdit_2->setText(ReadIPSettings());



}

MyDialog::~MyDialog()
{
    if (ui->lineEdit->text().length()>0) delete ui;
}

void MyDialog::on_pushButton_clicked()
{

    if (ui->lineEdit->text().length()>0)
    {
        WriteNameSettings(ui->lineEdit->text());
        accept();
    }
    if (ui->lineEdit_2->text().length()>0)
    {
        WriteIPSettings(ui->lineEdit_2->text());
        accept();
    }
}

QString MyDialog::getName()
{
    return ui->lineEdit->text();
}


QString MyDialog::getIP()
{
    return ui->lineEdit_2->text();
}


int MyDialog::getPort()
{
    return ui->lineEdit_3->text().toInt();
}

#endif
