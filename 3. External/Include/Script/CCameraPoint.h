#pragma once
#include <Engine/CScript.h>
class CCameraPoint :
    public CScript
{
private:
    int m_ePointType;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};

    CLONE(CCameraPoint);

public:
    CCameraPoint();
    CCameraPoint(const CCameraPoint& _origin);
    ~CCameraPoint();
};

