#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QtNetwork>
#include "dmgmeter.h"
#include "dmgmeter.cpp"
#include <sstream>
#include "global.h"
#include "QHBoxLayout"
#include "settings.h"
#include "ui_configurator.h"
#include "configurator.h"

using namespace GW2;

unsigned long m_Dps;
unsigned long m_5sDPS;
unsigned long m_realDps;
unsigned long m_Dmg;
unsigned long m_rDmg;
unsigned long m_Activity;
int LastColor;
unsigned long hitCounter;
unsigned long critCounter;
unsigned int m_critChance;
unsigned int m_healing;
unsigned long m_condiDmg;
unsigned long m_MaxDmg;
unsigned long m_MyProfession;
unsigned long countCombat;
int updateCounter;
QString combatCourse;
SHORT resetKeycode;
int resetMod;
SHORT combatmodeKeycode;
int combatmodeMod;
SHORT savelogKeycode;
int savelogMod;
SHORT globalresetKeycode;
int globalresetMod;
SHORT opacityKeycode;
int opacityMod;

bool combatModeToggleState;
bool shotcutsdisabled;

int main(int argc, char *argv[])
{

#ifdef QT_DEBUG
    qDebug() << "main:" << "thread id" << QThread::currentThreadId();
#endif // QT_DEBUG
    QApplication a(argc, argv);

    // get saved language
    QSettings settings("Gw2SPECS");
    settings.beginGroup("comboBoxLanguage");
    QString lang = settings.value("currentText").toString();
    settings.endGroup();

    QTranslator * qt_translator = new QTranslator;
    QString file_translator = QApplication::applicationDirPath() + "/languages/" + "gw2specs_" + lang + ".qm";

    if(!qt_translator->load(file_translator)){
        delete qt_translator;
    }else{
        qApp->installTranslator(qt_translator);
    }

    GW2::MainWindow w;
    w.show();

    return a.exec();
}


