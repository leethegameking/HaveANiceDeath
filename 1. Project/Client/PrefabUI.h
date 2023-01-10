#pragma once
#include "ResUI.h"
class PrefabUI :
    public ResUI
{
private:
    virtual void update() override;
    virtual void render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

