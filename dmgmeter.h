#ifndef DMGMETER_H
#define DMGMETER_H

//#define DMGMETER_DEBUG

#define DMGMETER_LOW_DPS_LIMIT 10000.0
#define DMGMETER_NORMAL_DPS_LIMIT 15000.0
#define DMGMETER_HIGH_DPS_LIMIT 200000.0
#define DMGMETER_LOW_MAX_DMG_LIMIT 2000
#define DMGMETER_NORMAL_MAX_DMG_LIMIT 4500
#define DMGMETER_HIGH_MAX_DMG_LIMIT 10000

#include <QTimer>
#include <QTime>
#include "imagereader.h"
#include <QTcpSocket>
#include <QDebug>
#include <global.h>

namespace GW2
{
    class DmgMeter : public QObject
    {
        Q_OBJECT

    signals:
        void RequestTimeUpdate(int);
        void RequestDmgUpdate(unsigned long long);
        void RequestDpsUpdate(int);
        void RequestMaxDmgUpdate(int);


    public slots:
        void SetUpdatesPerSecond(const QString& updatesPerSecond);
        void SetSecondsInCombat(const QString& secondsInCombat);
        void SetConsideredLineCount(const QString& consideredLineCount);
        void EvaluateImage(const QImage& image, const ImageAttributes& imageAttributes);
        void Reset();
        void SetIsAutoResetting(bool isAutoResetting);
        //void connectMe();

    public:
        DmgMeter();
        ~DmgMeter();

        inline int GetConsideredLineCount() const;
        inline bool IsActive() const;

        static const QString s_LowStyle;
        static const QString s_NormalStyle;
        static const QString s_HighStyle;
        static const QString s_UltraStyle;
        QTcpSocket *newSocketCon;

    private slots:
        void ComputeDps();

    private:
        void EvaluateLine(const QString& params);
        int ComputeDmg(const QString& dmgStr);
        void UpdateMaxDmg(int dmg);
        void StartEvaluation();

        QVector<QString> m_OldParams;
        QVector<QString> m_Params;
        int m_OldParamsMax;
        QTimer m_Timer;
        QTime OffCombatTime;
        QTime m_TimeSinceCombat;
        QTime m_TimeSinceEvaluation;

        ImageReader m_Reader;
        int OffCombatTimeInMsec;
        //int m_Activity;
        int m_ElapsedTimeSinceCombatInMsec;
        //int m_Dps;
        //unsigned long m_Dmg;
        int m_TimeoutInMsec;
        int m_SecsInCombat;
        bool m_IsActive;
        bool m_IsAutoResetting;
        char writeBuff[128];
        QByteArray tmp1;
        const char* tmp2;
        unsigned long LastDmg;
        int dmg_5s_ago;
        int dmg_4s_ago;
        int dmg_3s_ago;
        int dmg_2s_ago;
        int dmg_1s_ago;
        int dmg_now;

    };

    inline int DmgMeter::GetConsideredLineCount() const
    {
        return m_Params.size();
    }

    inline bool DmgMeter::IsActive() const
    {
        return m_IsActive;
    }
}



#endif // DMGMETER_H
