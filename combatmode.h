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

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void on_buttonResetCombatMode_clicked();

    private:
        Ui::CombatMode *ui;
        QPoint m_dragPosition;
        friend class MainWindow;
    };
}

#endif // COMBATMODE_H
