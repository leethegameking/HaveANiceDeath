#pragma once
#include "CUnitScript.h"

class CGameObject;

class CEnemyScript :
    public CUnitScript
{
protected:
    UINT    m_ePattern;

    CGameObject* m_pPlayerObj;

    Vec2    m_AppearRadius;
    Vec2    m_DetectRadius;

    bool    m_bAppear;
    bool    m_bDetect;

    

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

    CLONE(CEnemyScript);
public:
    CEnemyScript();
    CEnemyScript(int _iScriptType);
    CEnemyScript(const CEnemyScript& _origin);
    ~CEnemyScript();
};

