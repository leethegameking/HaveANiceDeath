#pragma once
#include <Engine/CScript.h>

class CPrefab;

enum ELEVATOR_STATE
{
    ELEVATOR_APPEAR,
    ELEVATOR_OPEN,
    ELEVATOR_WAIT,
    ELEVATOR_DISAPPEAR,
    ELEVATOR_CLOSE,

    ELEVATOR_WAIT_PLAYER,
    ELEVATOR_SELECT_LV,
};

class CElevatorScript :
    public CScript
{
private:
    int             m_eElevatorState;
    bool            m_bFirstState;

    float           m_fDetectRadius;
    Ptr<CPrefab>    m_pLVSelectPref;

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

