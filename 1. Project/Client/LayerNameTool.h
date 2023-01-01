#pragma once
#include "UI.h"

enum class LAYER_UI_MODE
{
    EDIT_MODE,
    COL_MODE,
};

class LayerNameTool :
    public UI
{
private:
    string  m_strIdx[MAX_LAYER];

    bool    m_bMode;

public:
    void update();
    void render_update();

private:
    void EditName();
    void SetCol();

public:
    LayerNameTool();
    ~LayerNameTool();
};

