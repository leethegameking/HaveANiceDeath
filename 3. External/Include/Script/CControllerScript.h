#pragma once
#include <Engine/CScript.h>

class CAnimController;

class CControllerScript :
    public CScript
{
public:
    CAnimController* m_sAnimCtrl;

    bool m_bJump;

    float m_fSpeedX;
    float m_fJumpSpeed;

    float m_fAccTime;

public:
    void begin() override;
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

