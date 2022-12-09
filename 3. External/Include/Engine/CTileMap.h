#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_AtlasTex;
    Vec2 m_vTileCount;

    Vec2 m_vSlice;

    // 나중에 2중벡터로 설계해보자
    vector <tTile> m_vecTile;
    CStructuredBuffer* m_TileBuffer;
     
    bool m_bDataChanged;

public:
    Ptr<CTexture> GetTileAtlas() { return m_AtlasTex; }
    void SetTileAtlas(Ptr<CTexture> _AtlasTex) { m_AtlasTex = _AtlasTex; }

    Vec2 GetTileCount() { return m_vTileCount; }
    void SetTileCount(UINT _iWidth, UINT _iHeight);

    Vec2 GetSlice() { return m_vSlice; }
    void SetSlice(Vec2 _slice) { m_vSlice = _slice; }

    vector<tTile>* GetTileInfo() { return &m_vecTile; }

    void DataChanged() { m_bDataChanged = true; }

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CTileMap);

public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};

