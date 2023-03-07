#pragma once
#include "CEnemyScript.h"
class CBrad :
    public CEnemyScript
{
private:
    UINT    m_eNextAttackPattern;
    UINT    m_ePrevAttackPattern;


    UINT    m_eNextDivePattern;

    float   m_fPoseMoveSpeed;
    float   m_fSpinMoveSpeed;
    float   m_fDiveMoveSpeed;

    float   m_fSpinAttackSpeed;
    float   m_fDiveAttackSpeed;

    Vec2    m_vToGroundSpeed;

    float   m_fDelayTime;

    float   m_fAerialHeight; // relative pos Campoint

    Ptr<CPrefab> m_FX_StompPref;
    Ptr<CPrefab> m_FX_FlashPref;

    bool        m_bOnce;

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

    void Brad_StompPose();
    void Brad_StompReady();
    void Brad_StompRock();
    void Brad_StompSpinAir(); //  -> Pose
    void Brad_StompSpinGround(); // -> Rock
    void Brad_StompToIdle();

    void Brad_StunStart();
    void Brad_StunLoop();
    void Brad_StunEnd();

    void Brad_Uturn();

    void Brad_Delay();

    void Brad_ToGround();


    // ========================================INTRO

    void Brad_IntroIdle();
    void Brad_IntroPingPong();
    void Brad_IntroMove();
    void Brad_IntroWelcome();
    void Brad_IntroToFight();

private:
    void SelectNextAttackPattern();
    void SelectNextDivePattern();
    void PlayRandomPose();
    void FlashFX();
    void StompFX();
    void CreateSpinRock();
    void MoveToTarget(Vec3 _vTarget, Vec2 _vSpeed, Vec3 _vDir);
    void MoveToTarget(Vec3 _vTarget, Vec2 _vSpeed);
    void MoveToTarget(Vec3 _vTarget, float _fSpeed);
    void MoveToTarget(Vec3 _vTarget, float _fSpeed, Vec3 _vDir);
    void MoveFromAnimInfo();
    void ColFromAnimInfo();
    void ColExpel();
    void ReverseDir();
};

