#pragma once
#include <Engine/CScript.h>

class CControllerScript :
    public CScript
{
public:
    bool m_bJump;

    float m_fSpeed;

    float m_fAccTime;

public:
    void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;


    CLONE(CControllerScript)
public:
    CControllerScript();
    ~CControllerScript();
};

