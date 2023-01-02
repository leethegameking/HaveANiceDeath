#pragma once
#include "CComponent.h"

enum RIGIDBODY_DIR
{
    RB_UP = 0x01,
    RB_DOWN = 0x02,
    RB_LEFT = 0x04,
    RB_RIGHT = 0x08,
};

class CRigidbody2D :
    public CComponent
{
private:
    Vec3 m_vPrevPos;
    // Vec3 m_vRealTimeWorldPos;

    Vec2 m_vForce;

    float m_fMass;

    Vec2 m_vSpeed;

    float m_fGravity;
    float m_fMaxGravitySpeed;

    bool m_bGround;
    bool m_bIgnoreGravity;

    float m_fMaxSpeed;

    short m_sDir;

    bool m_bForceSpeedX;
    bool m_bForceSpeedY;
    float m_fForceSpeedX;
    float m_fAccForceTime;
    float m_fMaxForceTime;

public:
    virtual void tick() override;
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }
    void SetForceSpeedX(float _ForceSpeedX, float _MaxTime = 0.f);
    void SetForceSpeedY(float _ForceSpeedY, float _MaxTime = 0.f);

public:
    bool IsGround() { return m_bGround; }
    void SetGround(bool _b) { m_bGround = _b; }

    short GetDir() { return m_sDir; }

    void SetPrevPos(Vec3 _PrevPos) { m_vPrevPos = _PrevPos; }
    Vec3 GetPrevPos() { return m_vPrevPos; }

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

