#include "pch.h"
#include "CWorkman.h"

CWorkman::CWorkman()
	: CEnemyScript((int)SCRIPT_TYPE::WORKMAN)
{
	m_CurUnitInfo.m_eName = UNIT_NAME::WORKMAN;
}

CWorkman::CWorkman(const CWorkman& _origin)
	: CEnemyScript(_origin)
{
}

CWorkman::~CWorkman()
{

}

void CWorkman::begin()
{
	CEnemyScript::begin();
	m_ePattern = WORK_MAN_WAITING;
}

void CWorkman::tick()
{
	CEnemyScript::tick();
	if (m_bAppear == false)
	{
		
	}

	switch (m_ePattern)
	{
	case WORK_MAN_WAITING:	
		break;
	case WORK_MAN_PATROL:
		break;
	case WORK_MAN_DELAY:
		break;
	case WORK_MAN_CHASE:
		break;
	case WORK_MAN_ATTACK:
		break;
	}
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


