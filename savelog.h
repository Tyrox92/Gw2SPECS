#ifndef SAVELOG_H
#define SAVELOG_H

#include <QDialog>

namespace Ui {
class saveLog;
}

namespace GW2
{
    class saveLog : public QDialog
    {
        Q_OBJECT

    public:
        explicit saveLog(QWidget *parent = 0);
        ~saveLog();
    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void on_saveTXT_clicked();

        void on_saveCSV_clicked();

    private:
        Ui::saveLog *ui;
        QPoint m_dragPosition;
        friend class MainWindow;
    };
}

#endif // SAVELOG_H
