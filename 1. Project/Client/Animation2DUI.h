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
    Ptr<CTexture>       m_AtlasTex;

    Vec2                m_vImageScale;

    vector<tAnim2DFrm>  m_vecFrm;
    int                 m_iCurIdx;

    ComboBox* m_AtlasComboBox;

    float               m_fAccTime;
    bool                m_bFinish;

    vector<string>      m_vecRes;

public:
    void SetAtlasTex(DWORD_PTR _texKey);

public:
    void init() override;
    void update() override;
    void render_update() override;
    void Close() override;

public:
    Animation2DUI();
    ~Animation2DUI();
};

