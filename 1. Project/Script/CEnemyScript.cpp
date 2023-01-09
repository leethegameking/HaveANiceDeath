#include "pch.h"
#include "CEnemyScript.h"

#include "CPlayerMgr.h"


CEnemyScript::CEnemyScript()
{
}

CEnemyScript::CEnemyScript(int _iScriptType)
	: CUnitScript(_iScriptType)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(200.f)
	, m_fDetectRadius(100.f)
	, m_fAttackRadius(50.f)
{

}

CEnemyScript::CEnemyScript(const CEnemyScript& _origin)
	: CUnitScript(_origin)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(200.f)
	, m_fDetectRadius(100.f)
	, m_fAttackRadius(50.f)
{
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::begin()
{
	CUnitScript::begin();
	// 
	m_pPlayerObj = CPlayerMgr::GetInst()->GetPlayerObj();

	// Collider 가져옴
	const vector<CGameObject*>& vecChildObj = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChildObj.size(); ++i)
	{
		if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::ENEMY_HIT)
		{
			m_pHitObj = vecChildObj[i];
		}
		else if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::ENEMY_ATTACK)
		{
			m_pAttObj = vecChildObj[i];
		}
	}

}

void CEnemyScript::tick()
{
	CUnitScript::tick();
}

void CEnemyScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CEnemyScript::Overlap(CCollider2D* _pOther)
{
}

void CEnemyScript::EndOverlap(CCollider2D* _pOther)
{
}

void CEnemyScript::SaveToFile(FILE* _pFile)
{
}

void CEnemyScript::LoadFromFile(FILE* _pFile)
{
}



