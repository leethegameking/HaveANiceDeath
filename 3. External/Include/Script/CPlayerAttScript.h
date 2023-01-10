#pragma once
#include "CAttScript.h"
class CPlayerAttScript :
    public CAttScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    CLONE(CPlayerAttScript)
public:
    CPlayerAttScript();
    CPlayerAttScript(const CPlayerAttScript& _origin);
    ~CPlayerAttScript();
};

