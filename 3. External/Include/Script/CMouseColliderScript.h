#pragma once
#include <Engine/CScript.h>

class CMouseColliderScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CMouseColliderScript);
public:
    CMouseColliderScript();
    CMouseColliderScript(const CMouseColliderScript& _origin);
    ~CMouseColliderScript();
};

