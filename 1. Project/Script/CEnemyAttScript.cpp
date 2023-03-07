#include "pch.h"
#include "CEnemyAttScript.h"

#include "CUnitScript.h" 

#include <Engine/CGameObject.h>

CEnemyAttScript::CEnemyAttScript()
	: CAttScript((int)SCRIPT_TYPE::ENEMYATTSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX_Att", &m_FX_Att);
}

CEnemyAttScript::CEnemyAttScript(const CEnemyAttScript& _origin)
	: CAttScript(_origin)
	, m_FX_Att(_origin.m_FX_Att)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX_Att", &m_FX_Att);
}

CEnemyAttScript::~CEnemyAttScript()
{
}

void CEnemyAttScript::FX_Att(CCollider2D* _other)
{
	if (m_FX_Att.Get())
	{
		if (_other->GetOwner()->GetParent() && _other->GetOwner()->GetParent()->GetScript<CUnitScript>())
		{
			CGameObject* pFXAtt = m_FX_Att->Instantiate();
			Vec3 vPos = Vec3(_other->GetFinalPos().x, _other->GetFinalPos().y, 0.f);
			Instantiate(pFXAtt, vPos);
		}
	}
}

void CEnemyAttScript::begin()
{
	CAttScript::begin();
}

void CEnemyAttScript::tick()
{
	CAttScript::tick();
}

void CEnemyAttScript::SaveToFile(FILE* _pFile)
{
	CAttScript::SaveToFile(_pFile);
	SaveResourceRef(m_FX_Att, _pFile);
}

void CEnemyAttScript::LoadFromFile(FILE* _pFile)
{
	CAttScript::LoadFromFile(_pFile);
	LoadResourceRef(m_FX_Att, _pFile);
}

void CEnemyAttScript::BeginOverlap(CCollider2D* _other)
{
	CAttScript::BeginOverlap(_other);
	FX_Att(_other);
}

void CEnemyAttScript::Overlap(CCollider2D* _other)
{
	CAttScript::Overlap(_other);
}

void CEnemyAttScript::EndOverlap(CCollider2D* _other)
{
	CAttScript::EndOverlap(_other);
}


