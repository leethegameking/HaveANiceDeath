#pragma once
#include <Engine/CScript.h>

class CAnimController;

class CUnitScript :
    public CScript
{
protected:
    tUnitInfo m_CurUnitInfo;
    tUnitInfo m_PrevUnitInfo;
    CAnimController* m_pAnimCon;
    tKnockBack       m_tKnockBack;
    
    vector<CGameObject*> m_vecGroundObj;

public:
    tUnitInfo& GetUnitInfo() { return m_CurUnitInfo; }
    UINT GetUnitState() { return m_CurUnitInfo.m_iStateBits; }
    void SetUnitState(UINT _Statebit) { m_CurUnitInfo.m_iStateBits = _Statebit; }
    void RemoveUnitState(UINT _Statebit) { m_CurUnitInfo.m_iStateBits &= ~_Statebit; }
    void AddUnitState(UINT _StateBits) { AddBit(m_CurUnitInfo.m_iStateBits, _StateBits); }

    void PushGroundObj(CGameObject* _pObj);             
    void SetGroundObj(CGameObject* _pObj, int _idx);    
    vector<CGameObject*>& GetGroundObjVec();            

    void SetKnockBack(tKnockBack _tKnockback) { m_tKnockBack = _tKnockback; }

private:
    void KnockBackProgress();

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
    CUnitScript(const CUnitScript& _origin);
    ~CUnitScript();
};

