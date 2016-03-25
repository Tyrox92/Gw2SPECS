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
    ui->InputName->setText(ReadNameSettings());
    ui->InputIP->setText(ReadIPSettings());
    ui->InputPort->setText(ReadPortSettings());
    Settings::ReadSettings(ui->professionComboBox);
}

MyDialog::~MyDialog()
{
    Settings::WriteSettings(ui->professionComboBox);
    if (ui->InputName->text().length()>0) delete ui;
}

void MyDialog::on_pushButton_clicked()
{

    if (ui->InputName->text().length()>0)
    {
        WriteNameSettings(ui->InputName->text());
        accept();
    }
    if (ui->InputIP->text().length()>0)
    {
        WriteIPSettings(ui->InputIP->text());
        accept();
    }
    if (ui->InputPort->text().length()>0)
    {
        WritePortSettings(ui->InputPort->text());
        accept();
    }
}

int MyDialog::getProfession()
{
    return ui->professionComboBox->currentIndex();
}

QString MyDialog::getName()
{
    return ui->InputName->text();
}


QString MyDialog::getIP()
{
    return ui->InputIP->text();
}


QString MyDialog::getPort()
{
    return ui->InputPort->text();
}

#endif
