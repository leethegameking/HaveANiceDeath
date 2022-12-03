#pragma once
#include "ResUI.h"
class TextureUI :
    public ResUI
{
private:


public:


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    TextureUI();
    ~TextureUI();
};

