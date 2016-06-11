#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include <QDialog>

namespace Ui {
class firstStart;
}

namespace GW2
{
    class firstStart : public QDialog
    {
        Q_OBJECT

    public:
        explicit firstStart(QWidget *parent = 0);
        ~firstStart();

    private:
        Ui::firstStart *ui;

        friend class MainWindow;
    };
}

#endif // FIRSTSTART_H
