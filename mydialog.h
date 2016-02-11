#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {

class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = 0);
    ~MyDialog();
    QString getName();
    QString getIP();
    int getProf();
    int getPort();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyDialog *ui;
};

#endif // MYDIALOG_Hs
