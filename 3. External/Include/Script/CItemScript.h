#pragma once
#include <Engine/CScript.h>
class CItemScript :
    public CScript
{
private:
    float m_fDelayTime;
    float m_fAccTime;

    Ptr<CPrefab> m_Transition;

    bool b;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CItemScript);
public:
    CItemScript();
    CItemScript(const CItemScript& _origin);
    ~CItemScript();
};


