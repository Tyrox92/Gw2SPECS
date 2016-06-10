#ifndef SHOWHIGHLIGHTEDPOPUP_H
#define SHOWHIGHLIGHTEDPOPUP_H

#include <QDialog>

namespace Ui {
class showhighlightedpopup;
}

namespace GW2
{
    class showhighlightedpopup : public QDialog
    {
        Q_OBJECT

    public:
        explicit showhighlightedpopup(QWidget *parent = 0);
        ~showhighlightedpopup();

    public slots:
        void doNotFocus();

    private:
        Ui::showhighlightedpopup *ui;
        friend class MainWindow;


    };
}
#endif // SHOWHIGHLIGHTEDPOPUP_H
