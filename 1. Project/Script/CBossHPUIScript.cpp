#include "pch.h"
#include "CBossHPUIScript.h"

#include "CUnitScript.h"
#include "CPlayerMgr.h"
#include <Engine/SimpleText.h>

CBossHPUIScript::CBossHPUIScript()
	: CScript((int)SCRIPT_TYPE::BOSSHPUISCRIPT)
	, m_bFirstTick(true)
{
}

CBossHPUIScript::CBossHPUIScript(const CBossHPUIScript& _origin)
	: CScript(_origin)
	, m_bFirstTick(true)
{
}

CBossHPUIScript::~CBossHPUIScript()
{
}

void CBossHPUIScript::begin()
{
}

void CBossHPUIScript::tick()
{
	if (m_bFirstTick)
	{
		m_pBossScript = CPlayerMgr::GetInst()->GetBossObj()->GetScript<CUnitScript>();
		m_bFirstTick = false;
	}

	m_fHPRatio = m_pBossScript->GetUnitInfo().m_fHP / m_pBossScript->GetUnitInfo().m_fMaxHP;
	MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_0, (void*)&m_fHPRatio);
}

void CBossHPUIScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CBossHPUIScript::Overlap(CCollider2D* _pOther)
{
}

void CBossHPUIScript::EndOverlap(CCollider2D* _pOther)
{
}

void CBossHPUIScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CBossHPUIScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}



