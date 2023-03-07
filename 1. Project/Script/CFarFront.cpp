#include "pch.h"
#include "CFarFront.h"

#include "CCameraMgr.h"
#include <Engine/CGameObject.h>

CFarFront::CFarFront()
	: CScript((int)SCRIPT_TYPE::FARFRONT)
{
}

CFarFront::CFarFront(const CFarFront& _origin)
	: CScript(_origin)
{
}

CFarFront::~CFarFront()
{
	m_bFirstTick = true;
}

void CFarFront::begin()
{
}

void CFarFront::tick()
{
	if (m_bFirstTick)
	{
		m_vPrevPos = CCameraMgr::GetInst()->GetMainCamera()->Transform()->GetRelativePos();
		m_vCurPos = m_vPrevPos;
		m_bFirstTick = false;
	}
	Vec3 vPos = CCameraMgr::GetInst()->GetMainCamera()->Transform()->GetRelativePos();
	Transform()->AddRelativePos(-(vPos - m_vPrevPos) / 15.f);
	m_vPrevPos = vPos;
}

void CFarFront::BeginOverlap(CCollider2D* _pOther)
{
}

void CFarFront::Overlap(CCollider2D* _pOther)
{
}

void CFarFront::EndOverlap(CCollider2D* _pOther)
{
}

void CFarFront::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CFarFront::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}


