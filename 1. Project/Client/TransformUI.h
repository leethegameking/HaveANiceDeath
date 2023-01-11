#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private:
    Vec3 m_vPos;
    Vec3 m_vScale;
    Vec3 m_vRot;

    Vec3 m_vWorldPos;
    Vec3 m_vWorldScale;
    Vec3 m_vWorldRot;

    bool m_bIgnScale;
    bool m_bMove;

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

