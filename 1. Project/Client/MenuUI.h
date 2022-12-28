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
    void CreateNewLevel(DWORD_PTR _LevelName);
    static void SaveCurLevel();


public:
    MenuUI();
    ~MenuUI();
};

