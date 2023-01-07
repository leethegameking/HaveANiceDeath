#include "pch.h"
#include "CWorkman.h"

CWorkman::CWorkman()
	:CUnitScript((int)SCRIPT_TYPE::WORKMAN)
{
	m_UnitInfo.m_eName = UNIT_NAME::WORKMAN;
}

CWorkman::CWorkman(const CWorkman& _origin)
	: CUnitScript(_origin)
{
}

CWorkman::~CWorkman()
{

}

void CWorkman::begin()
{
}

void CWorkman::tick()
{
}

void CWorkman::BeginOverlap(CCollider2D* _pOther)
{
}

void CWorkman::Overlap(CCollider2D* _pOther)
{
}

void CWorkman::EndOverlap(CCollider2D* _pOther)
{
}

void CWorkman::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CWorkman::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}


