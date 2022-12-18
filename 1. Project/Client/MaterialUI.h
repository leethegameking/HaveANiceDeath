#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{
private:
    TEX_PARAM   m_eSelectTexParam;

public:
    void SetTexture(DWORD_PTR _strTexKey);

private:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetGraphicsShader(DWORD_PTR _ShaderKey);

public:
    MaterialUI();
    ~MaterialUI();
};

