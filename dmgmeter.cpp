#include "dmgmeter.h"
#include <QString>
#include "global.h"
#include "mydialog.h"

using namespace GW2;

const QString DmgMeter::s_LowStyle = "color: rgb(255, 255, 255);";
const QString DmgMeter::s_NormalStyle = "color: rgb(0, 255, 0);";
const QString DmgMeter::s_HighStyle = "color: rgb(255, 165, 0);";
const QString DmgMeter::s_UltraStyle = "color: rgb(255, 128, 128);";


void DmgMeter::SetUpdatesPerSecond(const QString& updatesPerSecond)
{
    if (updatesPerSecond == "max")
    {
        // Update as fast es possible
        m_TimeoutInMsec = 1;
    }
    else
    {
        m_TimeoutInMsec = 1000 / updatesPerSecond.toInt();
    }
}

void DmgMeter::SetSecondsInCombat(const QString& secondsInCombat)
{
    m_SecsInCombat = secondsInCombat.toInt();
}

void DmgMeter::SetConsideredLineCount(const QString& consideredLineCount)
{
    m_Params.resize(consideredLineCount.toInt());
    m_OldParams.resize(m_Params.size());
    m_OldParamsMax=0;
}

void DmgMeter::EvaluateImage(const QImage& image, const ImageAttributes& imageAttributes)
{
    int offset = 0;
    int offsetOld = 0;
    int lastDiff = 0;
    int i;
    const int paramCount = m_Params.size();
    for (i = 0; i < paramCount; ++i)
    {
        const QString params = m_Reader.ReadLineFromBottom(image, imageAttributes, paramCount, i);
        if (params == "")
        {
            ++offset;
            ++offsetOld;
        }
        else
        {
            m_Params[i - offset] = params;
            if (m_OldParams[i - offsetOld] != params)
            {
                // Found valid difference, evaluate
                EvaluateLine(params);
                ++offsetOld;
                lastDiff = 1;
            } else lastDiff = 0;
        }
        if ((lastDiff==0) && (i-offsetOld>=m_OldParamsMax))  break;
    }
    if (i-offset!=0)
        {
        m_OldParamsMax=i-offset;
        m_Params.swap(m_OldParams);
        }
    if (m_IsActive)
    {
        emit RequestTimeUpdate(m_ElapsedTimeSinceCombatInMsec + m_TimeSinceCombat.elapsed());
    }
}

void DmgMeter::Reset()
{
    hitCounter=0;
    critCounter=0;
    m_condiDmg=0;
    LastColor=0;
    m_critChance=0;
    m_Dmg = 0;
    m_Dps = 0;
    m_MaxDmg = 0;
    m_ElapsedTimeSinceCombatInMsec = 0;
    OffCombatTimeInMsec=0;
    m_IsActive=0;
    m_Activity=0;
    m_TimeSinceCombat.start();
    OffCombatTime.start();
    emit RequestTimeUpdate(0);
    countCombat = 0;
    combatCourse = "";
    updateCounter=0;
    m_rDmg=0;
    m_5sDPS=0;
    m_realDps=0;
    dmg_5s_ago=0;
    dmg_4s_ago=0;
    dmg_3s_ago=0;
    dmg_2s_ago=0;
    dmg_1s_ago=0;
    dmg_now=0;
    m_OldParamsMax=0;
}

void DmgMeter::SetIsAutoResetting(bool isAutoResetting)
{
    m_IsAutoResetting = isAutoResetting;
    if (isAutoResetting && (m_TimeSinceEvaluation.elapsed() / 1000.0f) >= m_SecsInCombat)
    {
        Reset();
    }
}

DmgMeter::DmgMeter() :
    m_Timer(this),
    m_ElapsedTimeSinceCombatInMsec(0),
    //m_Dps(0),
    //m_Dmg(0),
    //m_MaxDmg(0),
    m_TimeoutInMsec(0),
    m_SecsInCombat(0),
    m_IsActive(false),
    m_IsAutoResetting(false),
    OffCombatTimeInMsec(0),
    dmg_5s_ago(0),
    dmg_4s_ago(0),
    dmg_3s_ago(0),
    dmg_2s_ago(0),
    dmg_1s_ago(0),
    dmg_now(0)
{
    QObject::connect(&m_Timer, SIGNAL(timeout()), this, SLOT(ComputeDps()));

    //    ImageAttributes a;
    //    QImage image("../../Screenshots/screen2png.png");

    //    m_Reader.UpdateImageAttributes(a, image);
    //    m_Reader.Read(image, a);
    //    m_Reader.ReadLineFromBottom(image, a, 0);

}

DmgMeter::~DmgMeter()
{
}

void DmgMeter::ComputeDps()
{
    const double elapsedTimeSinceCombat = (m_ElapsedTimeSinceCombatInMsec + m_TimeSinceCombat.elapsed()) ;
    const double elapsedSecsSinceCombat = elapsedTimeSinceCombat / 1000.0f;
    const double elapsedSecsSinceEvaluation = m_TimeSinceEvaluation.elapsed() / 1000.0f;
    m_Dps = elapsedSecsSinceCombat == 0.0 ? m_Dmg : m_Dmg / elapsedSecsSinceCombat; // Prevent division by zero
    if (m_Dps>999999) m_Dps = 1;
//    if (updateCounter<5) updateCounter++;     //5sec for recent DPS update
//        else
//            {
//            updateCounter=0;
//            m_5sDPS = elapsedSecsSinceCombat == 0.0 ? (m_Dmg-m_rDmg) : (m_Dmg-m_rDmg) / 5.0;
//            m_rDmg=m_Dmg;
//            }

    m_rDmg=m_Dmg;
    //m_realDps = dmg_now - dmg_1s_ago;

    dmg_5s_ago = dmg_4s_ago;
    dmg_4s_ago = dmg_3s_ago;
    dmg_3s_ago = dmg_2s_ago;
    dmg_2s_ago = dmg_1s_ago;
    dmg_1s_ago = dmg_now;
    dmg_now = m_rDmg;

    m_5sDPS =(5.0f/15.0f*(dmg_now-dmg_1s_ago)+4.0f/15.0f*(dmg_1s_ago-dmg_2s_ago)+3.0f/15.0f*(dmg_2s_ago-dmg_3s_ago)+2.0f/15.0f*(dmg_3s_ago-dmg_4s_ago)+1.0f/15.0f*(dmg_4s_ago-dmg_5s_ago));
    if(m_5sDPS>999999999){m_5sDPS=0;}
    if(m_realDps>999999999){m_realDps=0;}

    m_Activity=100.0f*elapsedTimeSinceCombat/(OffCombatTimeInMsec+elapsedTimeSinceCombat+1);
    if (m_Activity>100) m_Activity = 100;
    if (elapsedSecsSinceEvaluation >= m_SecsInCombat)
    {
        // No data received since m_SecsInCombat. End evaluation
        m_Timer.stop();
        m_ElapsedTimeSinceCombatInMsec += m_TimeSinceCombat.elapsed();
        emit RequestTimeUpdate(m_ElapsedTimeSinceCombatInMsec);
        m_IsActive = false;
        OffCombatTime.start();
        countCombat = 0; // We're currently not in combat
        if (m_IsAutoResetting)
        {
            Reset();
        }
    }
}

void DmgMeter::EvaluateLine(const QString& params)
{
    unsigned long dmg = ComputeDmg(params);

    if (LastColor>0)
    {
        if (LastColor==1)
        {
            hitCounter++;
            critCounter++;
            m_critChance=critCounter*100/hitCounter;
        }
        if (LastColor==2)
        {
            hitCounter++;
        }
        if (LastColor==3)
        {
            m_condiDmg+=dmg;
        }
    }
    if (dmg > m_MaxDmg)
    {
        // New max dmg found, set it as max dmg
        m_MaxDmg=dmg;
    }

#ifdef DMGMETER_DEBUG
    if (dmg < 100)
    {
        // Dmg low, could be wrong
        qDebug() << "Adding dmg:" << dmg;
    }
#endif // DMGMETER_DEBUG

    if (LastColor!=4)  //not timestamp color
    {
    m_Dmg += dmg;
    LastDmg=dmg;
    //Adding Each Damage Value done between seconds into the txtFileExport
    //combatCourse+="..   |  ?   |  ?  | +"+QString::number(dmg)+ "\r\n";
    combatCourse+="+"+QString::number(dmg)+"\r\n";
    //qDebug() << "Adding value : " << dmg;
    m_TimeSinceEvaluation.start();

    if (!m_IsActive)
    {
        // Evaluation starts, configure timer and start
        StartEvaluation();
    }
    }
   if (LastColor==4) qDebug() << "Skipping TimeStamp value : " << dmg;
   m_realDps += dmg;
}

int DmgMeter::ComputeDmg(const QString& dmgStr)
{
    unsigned int dmg = 0;
    const int size = dmgStr.size();
    for (int i = 0; i < size; ++i)
    {
        const QChar& c = dmgStr[i];
        if (c.isDigit())
        {
            // Param is digit, append it as dmg
            dmg *= 10;
            dmg += c.digitValue();
        }
    }
    return dmg;
}


void DmgMeter::StartEvaluation()
{
    if (m_Dmg-LastDmg==0)
    {
        OffCombatTimeInMsec=0;
        m_Activity=100;
        OffCombatTime.restart();
    } else OffCombatTimeInMsec+=OffCombatTime.elapsed();
    m_IsActive = true;
    m_TimeSinceCombat.start();
    m_Timer.start(m_TimeoutInMsec);
    //m_Dps = m_Dmg;

    //Setting countCombat to 1 to verify we're actively in combat
    countCombat = 1;
}


