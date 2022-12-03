#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class ListUI;
class ComboBox;

class TileMapUI :
    public ComponentUI
{
private:
    Ptr<CTexture> m_AtlasTex;
    Ptr<CTexture> m_SelectedMark;

    ComboBox*  m_AtlasComboBox;

    Vec2 m_vTileCount;
    Vec2 m_vImageScale;
    Vec2 m_vSlice;

    vector<tTile>* m_vecTile;
    vector<string> m_vecRes;

    Vec2 m_SelectedTexUV;
    Vec2 m_SelectedTexIdx;
    

public:
    void SetAtlasTex(DWORD_PTR _texKey);
    void SelectImageTile(Vec2 _vCursorPos);
    void ArrangeTile();

public:
    void init() override;
    void update() override;
    void render_update() override;


public:
    TileMapUI();
    ~TileMapUI();
};

