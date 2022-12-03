#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class ComboBox;
class CAnimator2D;

class Animation2DUI :
    public UI
{
private:
    vector<tAnim2DFrm>  m_vecFrm;
    int                 m_iCurIdx;

    CAnimator2D* m_pOwner;
    Ptr<CTexture>       m_AtlasTex;

    ComboBox* m_AtlasComboBox;

    float               m_fAccTime;
    bool                m_bFinish;

    vector<string>      m_vecRes;

public:
    void init() override;
    void update() override;
    void render_update() override;

public:
    Animation2DUI();
    ~Animation2DUI();
};

