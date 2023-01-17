#pragma once
#include <Engine/CScript.h>

class CLevelSelectScript :
    public CScript
{
private:
    string m_strLevelPath;

    CGameObject* m_pBossIcon;

    bool         m_bFirstTick;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CLevelSelectScript);
public:
    CLevelSelectScript();
    CLevelSelectScript(const CLevelSelectScript& _origin);
    ~CLevelSelectScript();
};

