#ifndef STARTSERVER_H
#define STARTSERVER_H

#include <QDialog>

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

    private slots:
        void on_keySequenceEdit_editingFinished();

    private:
        Ui::startserver *ui;
        friend class MainWindow;
    };
}
#endif // STARTSERVER_H
