#pragma once
#include "CEnemyScript.h"
class CBrad :
    public CEnemyScript
{
private:
    UINT    m_eNextAttackPattern;
    UINT    m_eNextDivePattern;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CBrad);
public:
    CBrad();
    CBrad(const CBrad& _origin);
    ~CBrad();

private:
    void Brad_Chase();

    void Brad_Combo1();
    void Brad_Combo1Start();
    void Brad_Combo2();
    void Brad_Combo2Start();
    void Brad_Combo2End();
    void Brad_Combo3();
    void Brad_Combo3Start();

    void Brad_Death();

    void Brad_DiveMove();
    void Brad_DiveReady();
    void Brad_DiveLoop();
    void Brad_DiveEnd();

    void Brad_Hit();

    void Brad_IntroIdle();
    void Brad_IntroDesk();
    void Brad_IntroMove();
    void Brad_IntroPingPong();
    void Brad_IntroToFight();
    void Brad_IntroWelcome();

    void Brad_StompPose();
    void Brad_StompReady();
    void Brad_StompRock();
    void Brad_StompSpinAir();
    void Brad_StompSpinGround();
    void Brad_StompToIdle();

    void Brad_StunStart();
    void Brad_StunLoop();
    void Brad_StunEnd();

    void Brad_Uturn();

    void Brad_Delay();

    void Brad_ToGround();

private:
    void SelectNextAttackPattern();
    void SelectNextDivePattern();
    void PlayRandomPose();
    void FlashFX();
    void StompFX();
    void CreateSpinRock();
    void MoveToTarget(Vec3 _vTarget, Vec2 _vSpeed, Vec3 _vDir);
    void MoveFromAnimInfo();
    void ReverseDir();
};

