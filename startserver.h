#ifndef STARTSERVER_H
#define STARTSERVER_H

#include <QDialog>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif

namespace Ui {
class startserver;
}
namespace GW2
{
    class startserver : public QDialog
    {
        Q_OBJECT

    public:
        explicit startserver(QWidget *parent = 0);
        ~startserver();

    //private slots:

    private slots:
        void on_pushButton_clicked();

    private:
        Ui::startserver *ui;
        friend class MainWindow;
    };
}
#endif // STARTSERVER_H
