﻿#include "pch.h"
#include "CPlayerAttScript.h"

#include "CEnemyScript.h"

CPlayerAttScript::CPlayerAttScript()
	: CAttScript((int)SCRIPT_TYPE::PLAYERATTSCRIPT)
{
}

CPlayerAttScript::CPlayerAttScript(const CPlayerAttScript& _origin)
	: CAttScript(_origin)
{
}

CPlayerAttScript::~CPlayerAttScript()
{
}

void CPlayerAttScript::begin()
{
	CAttScript::begin();
}

void CPlayerAttScript::tick()
{
	CAttScript::tick();
}

void CPlayerAttScript::SaveToFile(FILE* _pFile)
{
	CAttScript::SaveToFile(_pFile);
}

void CPlayerAttScript::LoadFromFile(FILE* _pFile)
{
	CAttScript::LoadFromFile(_pFile);
}

void CPlayerAttScript::BeginOverlap(CCollider2D* _other)
{
	CAttScript::BeginOverlap(_other);
	CEnemyScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CEnemyScript>();
	pTargetUnit->FillFaintGauge(3.f);
	if (CalBit(pTargetUnit->GetPattern(), PATTERN_HIT_START | PATTERN_HIT_LOOP | PATTERN_HIT_END, BIT_LEAST_ONE))
	{
		static tKnockBack tKB;
		tKB.bOn = true;
		tKB.eKnockBackDir = m_pUnitScr->GetUnitInfo().m_eDir;
		tKB.fDist = GetOwner()->GetParent()->Animator2D()->GetCurAnim()->GetPosChange().x;
		tKB.fDurationTime = 0.5f;
		pTargetUnit->SetKnockBack(tKB);
	}
}

void CPlayerAttScript::Overlap(CCollider2D* _other)
{
	CAttScript::Overlap(_other);
}

void CPlayerAttScript::EndOverlap(CCollider2D* _other)
{
	CAttScript::EndOverlap(_other);
}


