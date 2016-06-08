#include <QString>
#include <QTcpSocket>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif

#ifndef _GLOBALS_H
#define _GLOBALS_H

extern QString MyAuthCode;
extern QString MyName;
extern QString HostIP;
extern int HostPort;
extern unsigned long m_Dps;
extern unsigned long m_5sDPS;
extern unsigned long m_realDps;
extern unsigned long m_Dmg;
extern unsigned long m_rDmg;
extern unsigned long m_Activity;
extern int LastColor;
extern unsigned long hitCounter;
extern unsigned long critCounter;
extern unsigned long m_condiDmg;
extern unsigned int m_critChance;
extern unsigned int m_healing;
extern unsigned long m_MaxDmg;
extern unsigned long m_MyProfession;
extern unsigned long countCombat;
extern QString combatCourse;
extern int updateCounter;
extern int resetMod;
extern int combatmodeMod;
extern int savelogMod;
extern int globalresetMod;
extern int opacityMod;

#ifdef Q_OS_WIN
extern SHORT resetKeycode;
extern SHORT combatmodeKeycode;
extern SHORT savelogKeycode;
extern SHORT globalresetKeycode;
extern SHORT opacityKeycode;
#endif

extern bool combatModeToggleState;
extern bool shotcutsdisabled;






#endif
