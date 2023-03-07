#pragma once
#include "CAttScript.h"
class CEnemyAttScript :
    public CAttScript
{
private:
    Ptr<CPrefab> m_FX_Att;
    
private:
    void FX_Att(CCollider2D* _other);

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    CLONE(CEnemyAttScript)

public:
    CEnemyAttScript();
    CEnemyAttScript(const CEnemyAttScript& _origin);
    ~CEnemyAttScript();
};

