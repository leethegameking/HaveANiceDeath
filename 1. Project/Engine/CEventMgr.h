#pragma once
#include "singleton.h"


class CEventMgr :
    public CSingleton<CEventMgr>
{
private:
    vector<tEvent>          m_vecEvent;
    vector<CGameObject*>    m_vecGarbage;

    bool m_bLevelChanged;
    bool m_bResChanged;
    bool m_bResChangeFlag;

public:
    void tick();

public:
    void AddEvent(tEvent& _evn){m_vecEvent.push_back(_evn);}
    bool IsLevelChanged() { return m_bLevelChanged; }
    bool IsResChanged() { return m_bResChanged; }

    void ResChangeFlagOn() { m_bResChangeFlag = true; }

public:
    CEventMgr();
    ~CEventMgr();
};

