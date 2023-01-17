#pragma once
#include <Engine/CScript.h>

class CUnitScript;

class CBossHPUIScript:
    public CScript
{
private:
    CUnitScript* m_pBossScript;
    float        m_fHPRatio;

    bool        m_bFirstTick;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CBossHPUIScript);
public:
    CBossHPUIScript();
    CBossHPUIScript(const CBossHPUIScript& _origin);
    ~CBossHPUIScript();
};

