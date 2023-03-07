#include "pch.h"
#include "CCamPointWall.h"

#include "CCameraMgr.h"
#include <Engine/CGameObject.h>

CCamPointWall::CCamPointWall()
	: CScript((int)SCRIPT_TYPE::CAMPOINTWALL)
	, m_pColPref()
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "ColPref", &m_pColPref);
}

CCamPointWall::CCamPointWall(const CCamPointWall& _origin)
	: CScript(_origin)
	, m_pColPref(_origin.m_pColPref)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "ColPref", &m_pColPref);
}

CCamPointWall::~CCamPointWall()
{
}

void CCamPointWall::begin()
{
	m_bCreatWall	= false;
	m_bDeleteWall	= false;
}

void CCamPointWall::tick()
{
	if (m_bCreatWall)
	{
		CGameObject* pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
		Vec3 vObjPos = pCamPoint->Transform()->GetWorldPos();
		m_pWallLeft = m_pColPref->Instantiate();
		m_pWallRight = m_pColPref->Instantiate();

		Vec3 vObjLeft = Vec3(-800.f, 0.f, 0.f);
		Vec3 vObjRight = Vec3(800.f, 0.f, 0.f);

		Instantiate(m_pWallLeft, vObjPos + vObjLeft);
		Instantiate(m_pWallRight, vObjPos + vObjRight);

		m_bCreatWall = false;
	}

	if (m_bDeleteWall)
	{
		m_pWallLeft->Destroy();
		m_pWallRight->Destroy();

		CCameraMgr::GetInst();

		m_bDeleteWall = false;
	}
}

void CCamPointWall::BeginOverlap(CCollider2D* _pOther)
{
}

void CCamPointWall::Overlap(CCollider2D* _pOther)
{
}

void CCamPointWall::EndOverlap(CCollider2D* _pOther)
{
}

void CCamPointWall::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	SaveResourceRef(m_pColPref, _pFile);
}

void CCamPointWall::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadResourceRef(m_pColPref, _pFile);
}


