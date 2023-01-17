#pragma once
#include <Engine/CScript.h>

class CUnitScript;

class CPlayerHPUIScript :
    public CScript
{
private:
    CUnitScript* m_pPlayerScript;
    float        m_fHPRatio;

    bool        m_bFirstTick;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

private:


public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CPlayerHPUIScript);
public:
    CPlayerHPUIScript();
    CPlayerHPUIScript(const CPlayerHPUIScript& _origin);
    ~CPlayerHPUIScript();
};

