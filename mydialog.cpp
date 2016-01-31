#include "mydialog.h"
#include "ui_mydialog.h"
#include <QDebug>
#include <settings.h>

using namespace std;


MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(ReadNameSettings());
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
