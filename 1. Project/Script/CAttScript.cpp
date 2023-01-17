#include "pch.h"
#include "CAttScript.h"

#include "CUnitScript.h"
#include "CPlayerScript.h"
#include "CAnimController.h"
#include "CProjectileScript.h"

CAttScript::CAttScript()
	: CScript((int)SCRIPT_TYPE::ATTSCRIPT)
	, m_pUnitScr(nullptr)
	, m_pAnimCon(nullptr)
	, m_pProjScr(nullptr)
	, m_fCoefficient(1.f)
	, m_bFirstTick(true)
{
}

CAttScript::CAttScript(int _iScriptType)
	: CScript(_iScriptType)
	, m_pUnitScr(nullptr)
	, m_pAnimCon(nullptr)
	, m_pProjScr(nullptr)
	, m_fCoefficient(1.f)
	, m_bFirstTick(true)
{
}

CAttScript::~CAttScript()
{
}



void CAttScript::begin()
{

}

void CAttScript::tick()
{
	if (m_bFirstTick)
	{
		FirstTick();
	}

	// 공격 타입 설정.
	SetCoefficient();
}

void CAttScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile); 
}

void CAttScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}

void CAttScript::BeginOverlap(CCollider2D* _other)
{
	if (m_pUnitScr)
	{
		float pAtt = m_pUnitScr->GetUnitInfo().m_fAtt;
		CUnitScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CUnitScript>();
		if (pTargetUnit)
		{
			float& pTargetHP = pTargetUnit->GetUnitInfo().m_fHP;
			float  pTargetDef = pTargetUnit->GetUnitInfo().m_fDef;
			pTargetHP -= pAtt * m_fCoefficient - pTargetDef;
		}
	}
	else if(m_pProjScr)
	{
		float pAtt = m_pProjScr->GetAtt();
		if (_other->GetOwner()->GetParent())
		{
			CUnitScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CUnitScript>();
			if (pTargetUnit)
			{
				float& pTargetHP = pTargetUnit->GetUnitInfo().m_fHP;
				float  pTargetDef = pTargetUnit->GetUnitInfo().m_fDef;
				pTargetHP -= pAtt * m_fCoefficient - pTargetDef;
			}
		}
	}
}

void CAttScript::Overlap(CCollider2D* _other)
{
}

void CAttScript::EndOverlap(CCollider2D* _other)
{
}

void CAttScript::FirstTick()
{
	// 부모에서 받아와야함.
	if(GetOwner()->GetParent())
		m_pUnitScr = GetOwner()->GetParent()->GetScript<CUnitScript>();
	m_pProjScr = GetOwner()->GetScript<CProjectileScript>();
	m_bFirstTick = false;
}