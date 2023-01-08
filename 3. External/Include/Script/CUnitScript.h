#pragma once
#include <Engine/CScript.h>

enum UNIT_STATE
{
    UNIT_GROUND_PLATFORM    = 0x00000001,
    UNIT_GROUND             = 0x00000002,
    UNIT_HP_DOWN            = 0x00000004,
    UNIT_HP_ZERO            = 0x00000008
};



struct tUnitInfo
{
    float   m_fHP;
    float   m_fMP;
    float   m_fAtt;
    float   m_fDef;
    UINT    m_iStateBits;
    UNIT_NAME   m_eName;

    tUnitInfo()
        : m_fHP(100.f)
        , m_fMP(50.f)
        , m_fAtt(3.f)
        , m_fDef(1.f)
        , m_iStateBits(0)
        , m_eName(UNIT_NAME::END)
    {
    }
};

class CUnitScript :
    public CScript
{
protected:
    tUnitInfo m_CurUnitInfo;
    tUnitInfo m_PrevUnitInfo;

public:
    tUnitInfo& GetUnitInfo() { return m_CurUnitInfo; }
    UINT GetUnitState() { return m_CurUnitInfo.m_iStateBits; }
    void SetUnitState(UINT _Statebit) { m_CurUnitInfo.m_iStateBits = _Statebit; }
    void RemoveUnitState(UINT _Statebit) { m_CurUnitInfo.m_iStateBits &= ~_Statebit; }
    void AddUnitState(UINT _StateBits) { AddBit(m_CurUnitInfo.m_iStateBits, _StateBits); }

public:
    virtual void begin() override;
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

