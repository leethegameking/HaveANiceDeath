#pragma once
#include "CComponent.h"

enum RIGIDBODY_DIR
{
    RB_UP = 0x01,
    RB_DOWN = 0x02,
    RB_LEFT = 0x04,
    RB_RIGHT = 0x08,
};

struct tForceSpeed
{
    bool    Flag;
    float   fSpeed;
    float   fAccTime;
    float   fMaxTime;

    tForceSpeed()
        : Flag(false)
    {
       
    }
};

struct tAddForce
{
    bool    Flag;
    Vec2    vForce;

    float   fPrevAccTime;
    float   fAccTime;
    float   fMaxTime;
};

class CRigidbody2D :
    public CComponent
{
private:
    Vec3 m_vPrevPos;
    Vec2 m_vPrevColPos;
    // Vec3 m_vRealTimeWorldPos;

    Vec2 m_vForce;

    float m_fMass;

    Vec2 m_vSpeed;

    float m_fGravity; // 중력 가속도
    float m_fMaxGravitySpeed;

    bool m_bGround;
    bool m_bIgnoreGravity;

    float m_fMaxSpeed;

    short m_sDir;

    tForceSpeed m_ForceX;
    tForceSpeed m_ForceY;
    tAddForce   m_AddForce;


public:
    virtual void tick() override;
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _vForce, float _MaxTime = 0.f);
    void SetForceSpeedX(float _ForceSpeedX, float _MaxTime = 0.f);
    void SetForceSpeedY(float _ForceSpeedY, float _MaxTime = 0.f);

public:
    bool IsGround() { return m_bGround; }
    void SetGround(bool _b) { m_bGround = _b; }

    bool GetIgnGravity() { return m_bIgnoreGravity; }
    void SetIgnGravity(bool _b) { m_bIgnoreGravity = _b; }

    short GetDir() { return m_sDir; }

    void SetPrevPos(Vec3 _PrevPos) { m_vPrevPos = _PrevPos; }
    Vec3 GetPrevPos() { return m_vPrevPos; }
    Vec2 GetPrevColPos() { return m_vPrevColPos; }

    Vec2 GetSpeed() { return m_vSpeed; }

public:
    CLONE(CRigidbody2D);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CalcDir();

public:
    CRigidbody2D();
    ~CRigidbody2D();

    friend class RigidBody2DUI;
};

