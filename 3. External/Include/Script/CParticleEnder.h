#pragma once
#include <Engine/CScript.h>
class CParticleEnder :
    public CScript
{
private:
    float m_fMaxTime;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CParticleEnder);
public:
    CParticleEnder();
    CParticleEnder(const CParticleEnder& _origin);
    ~CParticleEnder();
};

