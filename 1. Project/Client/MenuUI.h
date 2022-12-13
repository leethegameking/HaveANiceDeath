#pragma once
#include "UI.h"
class MenuUI :
    public UI
{
private:


public:
    virtual void render();
    virtual void render_update();

public:
    MenuUI();
    ~MenuUI();
};

