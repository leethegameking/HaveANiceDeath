#pragma once
#include "UI.h"
class MenuUI :
    public UI
{
private:
    LEVEL_STATE m_ePrevState;

public:
    virtual void render();
    virtual void render_update();

public:
    MenuUI();
    ~MenuUI();
};

