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

    QRegExp nameRx("^[^;|*#]*$");
    QValidator *nameVal = new QRegExpValidator(nameRx, this);
    ui->InputName->setValidator(nameVal);
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::on_pushButton_clicked()
{

    if (ui->InputName->text().length()>=0)
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

QString MyDialog::getName()
{
    return ui->InputName->text().trimmed();
}


QString MyDialog::getIP()
{
    QRegExp whitespace("\\s");
    QString delwhitespace = ui->InputIP->text().trimmed();
    while (delwhitespace.contains(whitespace)) {
        delwhitespace = delwhitespace.left(delwhitespace.indexOf(whitespace)) + delwhitespace.right(delwhitespace.length()-delwhitespace.indexOf(whitespace)-1);
    }
    return delwhitespace;
}


QString MyDialog::getPort()
{
    QRegExp whitespace("\\s");
    QString delwhitespace = ui->InputPort->text().trimmed();
    while (delwhitespace.contains(whitespace)) {
        delwhitespace = delwhitespace.left(delwhitespace.indexOf(whitespace)) + delwhitespace.right(delwhitespace.length()-delwhitespace.indexOf(whitespace)-1);
    }
    return delwhitespace;
}

#endif
