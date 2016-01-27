#include "mydialog.h"
#include "ui_mydialog.h"
#include <QDebug>

using namespace std;


MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::on_pushButton_clicked()
{
    //QString MYNAME = ui->lineEdit->text();
   // accept();
    accept();
}

QString MyDialog::getName()
{
    return ui->lineEdit->text();
}

QString MyDialog::getIP()
{
    return ui->lineEdit_2->text();
}

