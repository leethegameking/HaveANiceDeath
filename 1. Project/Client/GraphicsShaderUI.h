#pragma once
#include "ResUI.h"
class GraphicsShaderUI :
    public ResUI
{
private:


public:


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    GraphicsShaderUI();
    ~GraphicsShaderUI();
};

