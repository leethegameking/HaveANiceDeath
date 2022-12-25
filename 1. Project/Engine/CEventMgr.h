#pragma once
#include "singleton.h"

class CLevel;
class CGameObject;

// typedef void (*SAVE_GAMEOBJECT)(CGameObject*, FILE*);
typedef CLevel* (*LOAD_LEVEL)(wstring _strRelativePath);

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
    static LOAD_LEVEL Load_Level_Func;

public:
    void tick();

public:
    void AddEvent(tEvent& _evn){m_vecEvent.push_back(_evn);}
    bool IsLevelChanged() { return m_bLevelChanged; }
    bool IsResChanged() { return m_bResChanged; }

    void ResChangeFlagOn() { m_bResChangeFlag = true; }
    void LevelChangFlagOn() { m_bLevelChangeFlag = true; }

    void SetLoadLevelFunc(LOAD_LEVEL _loadLevelFunc) { Load_Level_Func = _loadLevelFunc; }

public:
    CEventMgr();
    ~CEventMgr();
};

