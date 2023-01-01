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
    Vec3 m_vRealTimeWorldPos;

    Vec2 m_vForce;

    float m_fMass;


    Vec2 m_vSpeed;

    float m_fGravity;
    float m_fMaxGravitySpeed;

    bool m_bGround;
    bool m_bIgnoreGravity;

    float m_fMaxSpeed;

    short m_sDir;

public:
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }

public:
    bool IsGround() { return m_bGround; }
    void SetGround(bool _b) { m_bGround = _b; }

    short GetDir() { return m_sDir; }

    Vec3 GetPrevPos() { return m_vPrevPos; }

public:
    CLONE(CRigidbody2D);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CalcDir();

public:
    CRigidbody2D();
    ~CRigidbody2D();
};

