#pragma once
#include "ComponentUI.h"

class CAnimation2D;

class ComboBox;

class Animator2DUI :
    public ComponentUI
{
private:
    CAnimation2D*               m_pCurAnim;

    bool                        m_bRepeat;

    ComboBox*                   m_AnimComboBox;
    vector<string>              m_vecAnim;

public:
    void SetCurAnim(DWORD_PTR _animKey);

public:
    void init() override;
    void update() override;
    void render_update() override;

public:
    Animator2DUI();
    ~Animator2DUI();
};

