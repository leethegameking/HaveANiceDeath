#pragma once
#include "CTextScript.h"

class CUnitScript;

class CPlayerHPText :
    public CTextScript
{
private:
    CUnitScript* m_pUnitScr;

    bool m_bFirstTick;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CPlayerHPText);
public:
    CPlayerHPText();
    CPlayerHPText(const CPlayerHPText& _origin);
    ~CPlayerHPText();
};

