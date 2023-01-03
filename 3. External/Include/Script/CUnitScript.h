#pragma once
#include <Engine/CScript.h>

enum UNIT_STATE
{
    UNIT_AERIAL             = 0x0001,
    UNIT_GROUND             = 0x0002,
    UNIT_GROUND_PLATFORM    = 0x0004,
};

class CUnitScript :
    public CScript
{
    float   m_fHP;
    float   m_fMP;

    UINT    m_iStateBits;

public:
    int GetStateBits() { return m_iStateBits; }
    void AddStateBits(UINT _eState) 
    { 
        m_iStateBits |= _eState;
        int a = 0;
    }
    void RemoveStateBits(UINT _eState) { m_iStateBits &= ~_eState; }

public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    CLONE(CUnitScript)
public:
    CUnitScript();
    ~CUnitScript();
};

