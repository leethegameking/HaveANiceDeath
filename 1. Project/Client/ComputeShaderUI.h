#pragma once
#include "ResUI.h"
class ComputeShaderUI :
    public ResUI
{
private:


public:


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};

