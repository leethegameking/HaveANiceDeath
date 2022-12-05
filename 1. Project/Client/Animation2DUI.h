#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class ComboBox;
class CAnimation2D;
class CAnimator2D;

class Animation2DUI :
    public UI
{
private:
    Ptr<CTexture>       m_AtlasTex;

    Vec2                m_vImageScale;
    bool                m_bUniformed;
    Vec2                m_vSliceCount;

    bool                IsVertical;

    Vec2                m_vClickedIdx;
    bool                m_bHasSelected;

    ComboBox*           m_AtlasComboBox;

    vector<string>      m_vecRes;

public:
    Vec2 GetClickedIdx(Vec2 _vCursorPos);

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

