﻿#include "pch.h"
#include "CPlayerScript.h"



CPlayerScript::CPlayerScript()
	: CUnitScript(PLAYERSCRIPT)
{
	m_UnitInfo.m_eName = UNIT_NAME::PLAYER;
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CUnitScript(_origin)
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{

}

void CPlayerScript::tick()
{
}


void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}



void CPlayerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
