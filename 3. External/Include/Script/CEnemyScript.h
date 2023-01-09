#pragma once
#include "CUnitScript.h"

class CGameObject;

class CEnemyScript :
    public CUnitScript
{
protected:
    UINT    m_ePrevPattern;
    UINT    m_eCurPattern;

    CGameObject* m_pPlayerObj;

    CGameObject* m_pHitObj;
    CGameObject* m_pAttObj;

    float    m_fAppearRadius;
    float    m_fDetectRadius;
    float    m_fAttackRadius;



    

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    UINT GetPattern() { return m_eCurPattern; }
    void SetPattern(UINT _iPattern) { m_eCurPattern = _iPattern; }

    float GetAppearRadius() { return m_fAppearRadius; }
    float GetDetectRadius() { return m_fDetectRadius; }
    CGameObject* GetPlayerObj() { return m_pPlayerObj; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CEnemyScript);
public:
    CEnemyScript();
    CEnemyScript(int _iScriptType);
    CEnemyScript(const CEnemyScript& _origin);
    ~CEnemyScript();
};

