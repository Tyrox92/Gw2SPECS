#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <QDialog>
#include <QString>

namespace Ui {
    class authenticate;
}
namespace GW2
{
    class authenticate : public QDialog
    {
        Q_OBJECT

    public:
        explicit authenticate(QWidget *parent = 0);
        ~authenticate();
        QString getAuthCode();

    private slots:
        void on_authMe_pressed();

    private:
        Ui::authenticate *ui;
        friend class MainWindow;
    };
}
#endif // AUTHENTICATE_H
