#pragma once
#include <Engine/CScript.h>

class CPrefab;
class CLevelSelectScript;

enum ELEVATOR_STATE
{
    ELEVATOR_APPEAR,
    ELEVATOR_OPEN,
    ELEVATOR_WAIT,
    ELEVATOR_CLOSE,
    ELEVATOR_DISAPPEAR,


    ELEVATOR_WAIT_PLAYER,
    ELEVATOR_IN_RUN,
    ELEVATOR_IN,
    ELEVATOR_SELECT_LV,
    ELEVATOR_WAIT_2, // 2 -> ¥Ÿ∏• ∏ ¿∏∑Œ ¿Ãµø
    ELEVATOR_CLOSE_2,
};

class CElevatorScript :
    public CScript
{
private:
    int             m_eElevatorState;
    bool            m_bFirstState;

    float           m_fDetectRadius;
    Ptr<CPrefab>    m_pLVSelectPref; // æ» æ∏.
    bool            m_bSelectUIOn;

    CGameObject*    m_pPlayerObj;
    CGameObject*    m_pLVSelectUI;
    
    CLevelSelectScript* m_scrLVSelect;

    float           m_fDebugAddTime;

public:
    void SetLVSelectScr(CLevelSelectScript* _pScr) { m_scrLVSelect = _pScr; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CElevatorScript);
public:
    CElevatorScript();
    CElevatorScript(const CElevatorScript& _origin);
    ~CElevatorScript();
};

