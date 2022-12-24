#pragma once
#include "singleton.h"


class CEventMgr :
    public CSingleton<CEventMgr>
{
private:
    vector<tEvent>          m_vecEvent;
    vector<CGameObject*>    m_vecGarbage;
    vector<CGameObject*>    m_vecGrave;

    bool m_bLevelChanged;
    bool m_bResChanged;
    bool m_bResChangeFlag;
    bool m_bLevelChangeFlag;

public:
    void tick();

public:
    void AddEvent(tEvent& _evn){m_vecEvent.push_back(_evn);}
    bool IsLevelChanged() { return m_bLevelChanged; }
    bool IsResChanged() { return m_bResChanged; }

    void ResChangeFlagOn() { m_bResChangeFlag = true; }
    void LevelChangFlagOn() { m_bLevelChangeFlag = true; }

public:
    CEventMgr();
    ~CEventMgr();
};

