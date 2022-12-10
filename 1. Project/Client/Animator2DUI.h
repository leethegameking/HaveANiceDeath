#pragma once
#include "ComponentUI.h"

class CAnimation2D;

class ComboBox;

class Animator2DUI :
    public ComponentUI
{
private:
    Ptr<CAnimation2D>              m_pCurAnim;

    bool                        m_bRepeat;

    ComboBox*                   m_AnimComboBox;
    vector<string>              m_vecTargetAnim;
    vector<string>              m_vecResAnim;

    bool                        m_bTargetAnimChanged;

public:
    void SetCurAnim(DWORD_PTR _animKey);
    void AddAnimation(DWORD_PTR _multiSelectIdx);
    void SetVecTargetAnim();
    bool IsTargetAnimChanged() { return m_bTargetAnimChanged; }
    int CalComoboPreviewIdx();

public:
    void init() override;
    void update() override;
    void render_update() override;

public:
    Animator2DUI();
    ~Animator2DUI();

};

