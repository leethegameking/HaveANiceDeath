#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI* m_Tree;

public:
    void ResetContent();

private:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetResourceToInspector(DWORD_PTR _res);

public:
    ContentUI();
    ~ContentUI();
};

