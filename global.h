#include <QString>
#include <QTcpSocket>
#include <windows.h>


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
extern SHORT resetKeycode;
extern int combatmodeMod;
extern SHORT combatmodeKeycode;
extern int savelogMod;
extern SHORT savelogKeycode;
extern int globalresetMod;
extern SHORT globalresetKeycode;
extern bool combatModeToggleState;






#endif
