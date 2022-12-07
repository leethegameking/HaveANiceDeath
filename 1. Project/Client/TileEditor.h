#pragma once
#include "UI.h"
#include "TileMapUI.h"

class TileEditor :
    public UI
{
private:
    vector<tTile>   m_vecTileChange;
    TileMapUI*       m_TileMapUI;

    Vec2            m_vImageScale;

public:
    void init() override;
    void update() override;
    void render_update() override;
    void Close() override;

public:
    void SetTileMapUI(TileMapUI* _tileMapUI) { m_TileMapUI = _tileMapUI; }

public:
    TileEditor();
    ~TileEditor();
};

