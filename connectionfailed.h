#ifndef CONNECTIONFAILED_H
#define CONNECTIONFAILED_H

#include <QDialog>

namespace Ui {
class connectionfailed;
}

namespace GW2
{
    class connectionfailed : public QDialog
    {
        Q_OBJECT

    public:
        explicit connectionfailed(QWidget *parent = 0);
        ~connectionfailed();

    private:
        Ui::connectionfailed *ui;

        friend class MainWindow;
    };
}

#endif // CONNECTIONFAILED_H
