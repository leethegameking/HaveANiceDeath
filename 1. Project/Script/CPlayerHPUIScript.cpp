#include "pch.h"
#include "CPlayerHPUIScript.h"

#include "CUnitScript.h"
#include "CPlayerMgr.h"
#include <Engine/SimpleText.h>

CPlayerHPUIScript::CPlayerHPUIScript()
	: CScript((int)SCRIPT_TYPE::PLAYERHPUISCRIPT)
	, m_bFirstTick(true)
{
}

CPlayerHPUIScript::CPlayerHPUIScript(const CPlayerHPUIScript& _origin)
	: CScript(_origin)
	, m_bFirstTick(true)
{
}

CPlayerHPUIScript::~CPlayerHPUIScript()
{
}

void CPlayerHPUIScript::begin()
{
	
}

void CPlayerHPUIScript::tick()
{
	if (m_bFirstTick)
	{
		m_pPlayerScript = CPlayerMgr::GetInst()->GetPlayerObj()->GetScript<CUnitScript>();
		m_bFirstTick = false;
	}

	m_fHPRatio = m_pPlayerScript->GetUnitInfo().m_fHP / m_pPlayerScript->GetUnitInfo().m_fMaxHP;
	MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_0, (void*)&m_fHPRatio);
}

void CPlayerHPUIScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CPlayerHPUIScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerHPUIScript::EndOverlap(CCollider2D* _pOther)
{
}

void CPlayerHPUIScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CPlayerHPUIScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}



