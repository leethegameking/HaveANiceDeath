#pragma once
#include <Engine/CScript.h>

enum UNIT_STATE
{
    UNIT_GROUND_PLATFORM    = 0x00000001,
    UNIT_GROUND             = 0x00000002,
};

struct tUnitInfo
{
    float   m_fHP;
    float   m_fMP;
    float   m_fAtt;
    UINT    m_iStateBits;
};

class CUnitScript :
    public CScript
{
protected:
    tUnitInfo m_UnitInfo;

public:
    UINT GetUnitState() { return m_UnitInfo.m_iStateBits; }
    void AddUnitState(UINT _StateBits) { AddBit(m_UnitInfo.m_iStateBits, _StateBits); }
    void RemoveUnitState(UINT _StateBits){ RemoveBit(m_UnitInfo.m_iStateBits, _StateBits); }

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
    CUnitScript(int _ScriptType);
    ~CUnitScript();
};

