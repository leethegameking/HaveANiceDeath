#pragma once
#include "CUnitScript.h"

class CPrefab;
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

    float    m_fFaintTime;
    float    m_fFaintGauge;

    Ptr<CPrefab> m_pProjectile;

    Ptr<CPrefab> m_FX_Detect;
    Ptr<CPrefab> m_FX_Death_1;
    Ptr<CPrefab> m_FX_Death_2;
    Ptr<CPrefab> m_FX_StunStar;
    Ptr<CPrefab> m_FX_Alert;

protected:
    bool m_bUturn;
    bool m_bStateEnter;
    bool m_bDeath;

protected:
    virtual void WaitState() {};
    virtual void AppearSatae() {};
    virtual void IdleState() {};
    virtual void UturnState(){};
    virtual void DetectState(){};
    virtual void RunState(){};
    virtual void AttackReadyState(){};
    virtual void AttackState(){};
    virtual void HitStartState(){};
    virtual void HitLoopState(){};
    virtual void HitEndState(){};
    virtual void DelayState() {}; // 공격 후 쫓아가며
    virtual void DeathState() {};

protected:
    ANIM_DIR GetAnimDirToPlayer();
    Vec2 GetDirToPlayer();
    void SetDir(ANIM_DIR _eDir);
    bool CurAnimFinish();

    void FX_Death_1();
    void FX_Death_2();
    void FX_StunStar();
    void FX_Detect();
    void FX_Alert();

public:
    virtual void begin() override;
    virtual void tick() override;

    // Player와 충돌 시 밀어냄.
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    UINT GetPattern() { return m_eCurPattern; }
    void SetPattern(UINT _iPattern) { m_eCurPattern = _iPattern; }

    float GetAppearRadius() { return m_fAppearRadius; }
    float GetDetectRadius() { return m_fDetectRadius; }
    CGameObject* GetPlayerObj() { return m_pPlayerObj; }



    void FillFaintGauge(float _fill) { m_fFaintGauge -= _fill; }

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

