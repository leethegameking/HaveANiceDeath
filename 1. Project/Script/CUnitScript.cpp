#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript()
	: CScript((int)SCRIPT_TYPE::UNITSCRIPT)
	, m_fHP(100.f)
	, m_fMP(100.f)
	, m_iStateBits(0)
{
}

CUnitScript::~CUnitScript()
{
}



void CUnitScript::tick()
{
	int a = 0;
	a += 1;
}


void CUnitScript::BeginOverlap(CCollider2D* _other)
{
}

void CUnitScript::Overlap(CCollider2D* _other)
{
}

void CUnitScript::EndOverlap(CCollider2D* _other)
{
}


void CUnitScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CUnitScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
