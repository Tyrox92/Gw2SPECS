#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <QDialog>

namespace Ui {
class updateCheck;
}

namespace GW2
{
    class updateCheck : public QDialog
    {
        Q_OBJECT

    public:
        explicit updateCheck(QWidget *parent = 0);
        ~updateCheck();
    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private:
        Ui::updateCheck *ui;
        QPoint m_dragPosition;
        friend class MainWindow;
    };
}

#endif // UPDATECHECK_H
