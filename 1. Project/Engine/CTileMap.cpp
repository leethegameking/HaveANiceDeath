#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_bDataChanged(false)
	, m_vSlice(64.f)
	, m_vTileCount(10.f, 10.f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _origin)
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_bDataChanged(false)
	, m_vSlice(_origin.m_vSlice)
	, m_vTileCount(_origin.m_vTileCount)
	, m_AtlasTex(_origin.m_AtlasTex)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));
	m_vecTile.assign(_origin.m_vecTile.begin(), _origin.m_vecTile.end());

	m_TileBuffer = new CStructuredBuffer;

	SetTileCount((UINT)m_vTileCount.x, (UINT)m_vTileCount.y);
}

CTileMap::~CTileMap()
{
	if (m_TileBuffer)
		delete m_TileBuffer;
}

void CTileMap::SetTileCount(UINT _iWidth, UINT _iHeight)
{
	m_vTileCount = Vec2((float)_iWidth, (float)_iHeight);

	vector<tTile> tmpVecTile;
	tmpVecTile.assign(m_vecTile.begin(), m_vecTile.end());

	m_vecTile.clear();
	m_vecTile.resize(_iWidth * _iHeight);

	if (m_TileBuffer->GetElementCount() < _iWidth * _iHeight)
	{
		m_TileBuffer->Create(sizeof(tTile), _iWidth * _iHeight, SB_TYPE::SRV_ONLY, nullptr, true);
	}

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();

	m_vecTile.assign(m_vTileCount.x * m_vTileCount.y,
		tTile({ Vec2(0.f, 0.f), Vec2(m_vSlice.x / fWidth, m_vSlice.y / fHeight) }));

	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
}

void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	Transform()->UpdateData();

	if (m_bDataChanged)
	{
		float fWidth = (float)m_AtlasTex->GetWidth();
		float fHeight = (float)m_AtlasTex->GetHeight();

		for (int i = 0; i < m_vecTile.size(); ++i)
		{
			m_vecTile[i].vSlice = Vec2(m_vSlice.x / fWidth, m_vSlice.y / fHeight);
		}
		m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
	}
	m_TileBuffer->UpdateData(17, PIPELINE_STAGE::PS);

	GetCurMaterial()->SetScalarParam(VEC2_0, &m_vTileCount);
	GetCurMaterial()->SetTexParam(TEX_0, m_AtlasTex);
	GetCurMaterial()->UpdateData();

	GetMesh()->render();

	m_bDataChanged = false;
}


