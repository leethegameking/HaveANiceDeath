#pragma once
#include "CComponent.h"
class CRigidbody2D :
    public CComponent
{
private:
    Vec2 m_vForce;

    float m_fMass;


    Vec2 m_vSpeed;

    float m_fGravity;
    float m_fMaxGravitySpeed;

    bool m_bGround;
    bool m_bIgnoreGravity;

    float m_fMaxSpeed;

public:
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }

public:
    CRigidbody2D();
    ~CRigidbody2D();
};

