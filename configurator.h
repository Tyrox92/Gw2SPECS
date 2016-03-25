#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QDialog>

namespace Ui
{
    class Configurator;
}

namespace GW2
{
    class Configurator : public QDialog
    {
        Q_OBJECT

    public:
        explicit Configurator(QWidget *parent = 0);
        ~Configurator();

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void RestoreDefaults();

    private:
        Ui::Configurator *ui;
        QPoint m_dragPosition;
        friend class MainWindow;
    };
}

#endif // CONFIGURATOR_H
