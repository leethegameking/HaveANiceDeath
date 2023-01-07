#include "pch.h"
#include "CAttScript.h"

#include "CUnitScript.h"
#include "CPlayerScript.h"
#include "CAnimController.h"

CAttScript::CAttScript()
	: CScript((int)SCRIPT_TYPE::ATTSCRIPT)
	, m_pUnit(nullptr)
	, m_pAnimCon(nullptr)
	, m_fCoefficient(1.f)
{
}

CAttScript::~CAttScript()
{
}

void CAttScript::begin()
{
	// 부모에서 받아와야함.
	m_pUnit = GetOwner()->GetParent()->GetScript<CPlayerScript>(); assert(m_pUnit);
	/*m_pAnimCon = GetOwner()->GetScript<CAnimController>(); assert(m_pAnimCon);*/
}

void CAttScript::tick()
{
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
	float pAtt = m_pUnit->GetUnitInfo().m_fAtt;
	CUnitScript* pTargetUnit = _other->GetOwner()->GetScript<CUnitScript>();
	float& pTargetHP = pTargetUnit->GetUnitInfo().m_fHP;
	float  pTargetDef = pTargetUnit->GetUnitInfo().m_fDef;
	pTargetHP -= pAtt * m_fCoefficient - pTargetDef;
}

void CAttScript::Overlap(CCollider2D* _other)
{
}

void CAttScript::EndOverlap(CCollider2D* _other)
{
}


