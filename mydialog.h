#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {

class MyDialog;
}

namespace GW2
{
    class MyDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit MyDialog(QWidget *parent = 0);
        ~MyDialog();
        QString getName();
        QString getIP();
        QString getPort();

    private slots:
        void on_pushButton_clicked();

    private:
        Ui::MyDialog *ui;

        friend class MainWindow;
    };
}

#endif // MYDIALOG_Hs
