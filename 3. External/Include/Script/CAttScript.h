#pragma once
#include <Engine/CScript.h>

class CUnitScript;
class CAnimController;

class CAttScript :
    public CScript
{
private:
    CUnitScript*        m_pUnit;
    CAnimController*    m_pAnimCon;
    float               m_fCoefficient;

private:
    void SetCoefficient() {}; // 애니메이션에 따른 공격 계수를 구해줌. 

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    CLONE(CAttScript)
public:
    CAttScript();
    ~CAttScript();
};

