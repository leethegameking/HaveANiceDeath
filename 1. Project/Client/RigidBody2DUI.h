#pragma once
#include "ComponentUI.h"
class RigidBody2DUI :
    public ComponentUI
{
private:


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    RigidBody2DUI();
    ~RigidBody2DUI();
};

