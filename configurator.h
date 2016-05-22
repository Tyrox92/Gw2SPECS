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
        Ui::Configurator *ui;
    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void RestoreDefaults();
        void on_shortcut1_pressed();
        void listentoKeys();
        void on_seqEditCombatMode_editingFinished();
        void on_seqEditReset_editingFinished();
        void on_seqEditGlobalReset_editingFinished();
        void on_seqEditSave_editingFinished();

    private:

        QPoint m_dragPosition;
        friend class MainWindow;
    };
}

#endif // CONFIGURATOR_H
