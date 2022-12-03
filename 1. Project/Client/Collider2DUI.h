#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:
    Vec2            m_vOffsetPos;
    Vec2            m_vScale;
    Vec3            m_vRot;

    Vec2            m_vFinalPos;
    Vec2            m_vFinalScale;
    bool            m_bIgnorObjectScale;

    Vec4            m_vIdleColor;
    Vec4            m_vCollisionColor;
    int             m_iOverlapCount;

    bool            IsCollisionColor;

public:
    void update() override;
    void render_update() override;


public:
    Collider2DUI();
    ~Collider2DUI();
};

