#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class TileEditor;
class ListUI;
class ComboBox;

class TileMapUI :
    public ComponentUI
{
private:
    Ptr<CMesh>      m_Mesh;
    Ptr<CMaterial>  m_Mtrl;

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
    TileEditor* m_TileEditor;

    bool m_bInstanceMode;
    bool m_bTileChanged;

public:
    void SetAtlasTex(DWORD_PTR _texKey);
    void SelectImageTile(Vec2 _vCursorPos, Vec2 _vImageScale);
    void ArrangeTile();

public:
    void init() override;
    void update() override;
    void render_update() override;


public:
    TileMapUI();
    ~TileMapUI();

    friend class TileEditor;
};

