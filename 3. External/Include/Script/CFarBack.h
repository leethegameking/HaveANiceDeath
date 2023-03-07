#pragma once
#include <Engine/CScript.h>
class CFarBack :
    public CScript
{
private:
    Vec3 m_vPrevPos;
    Vec3 m_vCurPos;

    bool m_bFirstTick;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CFarBack);
public:
    CFarBack();
    CFarBack(const CFarBack& _origin);
    ~CFarBack();
};

