#ifndef COMBATMODE_H
#define COMBATMODE_H

#include <QDialog>

namespace Ui {
class CombatMode;
}

namespace GW2
{
    class CombatMode : public QDialog
    {
        Q_OBJECT

    public:
        explicit CombatMode(QWidget *parent = 0);
        ~CombatMode();

    private slots:
        void on_buttonResetCombatMode_clicked();

    private:
        Ui::CombatMode *ui;

        friend class MainWindow;
    };
}

#endif // COMBATMODE_H
