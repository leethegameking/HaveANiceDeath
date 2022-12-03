#pragma once
#include "CScript.h"
class CParticleHandler :
    public CScript
{

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;




    CLONE(CParticleHandler);
public:
    CParticleHandler();
    ~CParticleHandler();
};

