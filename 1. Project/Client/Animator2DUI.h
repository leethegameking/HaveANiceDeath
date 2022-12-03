#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
private:

public:
    void update() override;
    void render_update() override;

public:
    Animator2DUI();
    ~Animator2DUI();
};

