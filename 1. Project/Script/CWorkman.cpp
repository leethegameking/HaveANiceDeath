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
	m_ePattern = E_WAITING;
}

void CWorkman::tick()
{
	CEnemyScript::tick();

	DetectCheck();
	AppearCheck();

	switch (m_ePattern)
	{
	case E_WAITING: 
		break;
	case E_APPEAR:
		break;
	case E_IDLE:
		break;
	case E_UTURN:
		break;
	case E_DETECT:
		break;
	case E_RUN:
		break;
	case E_ATTACK:
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


