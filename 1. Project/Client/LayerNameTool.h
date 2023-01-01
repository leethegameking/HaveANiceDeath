#pragma once
#include "UI.h"
class LayerNameTool :
    public UI
{
private:
    string m_strIdx[MAX_LAYER];

public:
    void update();
    void render_update();


public:
    LayerNameTool();
    ~LayerNameTool();
};

