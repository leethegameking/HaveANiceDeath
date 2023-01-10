#pragma once
#include "CEnemyScript.h"



class CWorkman :
    public CEnemyScript
{
private:
    bool m_bUturn;
    bool m_bStateEnter;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

private:
    void WaitState();
    void AppearSatae();
    void IdleState();
    void UturnState();
    void DetectState();
    void RunState();
    void AttackReadyState();
    void AttackState();
    void HitStartState();
    void HitLoopState();
    void HitEndState();
    void DelayState(); // 공격 후 쫓아가며 


private:
    void SetDir(ANIM_DIR _eDir);
    bool CurAnimFinish();
    ANIM_DIR GetDirToPlayer();

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CWorkman);
public:
    CWorkman();
    CWorkman(const CWorkman& _origin);
    ~CWorkman();
};

