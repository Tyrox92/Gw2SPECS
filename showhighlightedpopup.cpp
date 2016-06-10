#include "showhighlightedpopup.h"
#include "ui_showhighlightedpopup.h"
#include "mainwindow.h"
#include <QDebug>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif

using namespace GW2;

showhighlightedpopup::showhighlightedpopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showhighlightedpopup)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

showhighlightedpopup::~showhighlightedpopup()
{
    delete ui;
}

void showhighlightedpopup::doNotFocus(){
    #ifdef Q_OS_WIN
    HWND windowsHandle  = (HWND)FindWindow(0, TEXT("PopupWindow"));
    SetForegroundWindow(windowsHandle);
    ShowWindow(windowsHandle, SW_HIDE);
    SetWindowLong(windowsHandle, GWL_EXSTYLE, GetWindowLong(windowsHandle, GWL_EXSTYLE)| WS_EX_NOACTIVATE | WS_EX_APPWINDOW | WS_EX_TRANSPARENT);
    SetWindowPos(windowsHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
    ShowWindow(windowsHandle, SW_SHOW);
    #endif
}
