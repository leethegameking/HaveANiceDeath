#include "pch.h"
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
}

void CPlayerAttScript::Overlap(CCollider2D* _other)
{
	CAttScript::Overlap(_other);
}

void CPlayerAttScript::EndOverlap(CCollider2D* _other)
{
	CAttScript::EndOverlap(_other);
}


