#pragma once
#include "CEnemyScript.h"
class CWorkwoman :
    public CEnemyScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

private:
    virtual void WaitState();
    virtual void AppearSatae();
    virtual void IdleState();
    virtual void UturnState();
    virtual void DetectState();
    virtual void RunState();
    virtual void AttackReadyState();
    virtual void AttackState();
    virtual void HitStartState();
    virtual void HitLoopState();
    virtual void HitEndState();
    virtual void DelayState(); // 공격 후 쫓아가며 
    virtual void DeathState();

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CWorkwoman);
public:
    CWorkwoman();
    CWorkwoman(const CWorkwoman& _origin);
    ~CWorkwoman();
};

