#include "pch.h"
#include "CFarBack.h"
#include "CCameraMgr.h"
#include <Engine/CGameObject.h>

CFarBack::CFarBack()
	: CScript((int)SCRIPT_TYPE::FARBACK)
{
}

CFarBack::CFarBack(const CFarBack& _origin)
	: CScript(_origin)
{
}

CFarBack::~CFarBack()
{
}

void CFarBack::begin()
{
	m_bFirstTick = true;
}

void CFarBack::tick()
{
	if (m_bFirstTick)
	{
		m_vPrevPos = CCameraMgr::GetInst()->GetMainCamera()->Transform()->GetRelativePos();
		m_vCurPos = m_vPrevPos;
		m_bFirstTick = false;
	}
	Vec3 vPos = CCameraMgr::GetInst()->GetMainCamera()->Transform()->GetRelativePos();
	Transform()->AddRelativePos((vPos - m_vPrevPos) / 15.f);
	m_vPrevPos = vPos;
}

void CFarBack::BeginOverlap(CCollider2D* _pOther)
{
}

void CFarBack::Overlap(CCollider2D* _pOther)
{
}

void CFarBack::EndOverlap(CCollider2D* _pOther)
{
}

void CFarBack::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CFarBack::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}


