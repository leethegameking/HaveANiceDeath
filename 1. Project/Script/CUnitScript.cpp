#include "pch.h"
#include "CUnitScript.h"

#include "CAnimController.h"

CUnitScript::CUnitScript()
	: CScript((int)SCRIPT_TYPE::UNITSCRIPT)
	, m_CurUnitInfo{}
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "HP    ", &m_CurUnitInfo.m_fHP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MP    ", &m_CurUnitInfo.m_fMP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att   ", &m_CurUnitInfo.m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Def   ", &m_CurUnitInfo.m_fDef);
	AddScriptParam(SCRIPT_PARAM::INT,	"State ", &m_CurUnitInfo.m_iStateBits);
}

CUnitScript::CUnitScript(int _ScriptType)
	: CScript(_ScriptType)
	, m_CurUnitInfo{}
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "HP    ", &m_CurUnitInfo.m_fHP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MP    ", &m_CurUnitInfo.m_fMP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att   ", &m_CurUnitInfo.m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Def   ", &m_CurUnitInfo.m_fDef);
	AddScriptParam(SCRIPT_PARAM::INT, "State ", &m_CurUnitInfo.m_iStateBits);
}

CUnitScript::~CUnitScript()
{
}



void CUnitScript::begin()
{
	m_PrevUnitInfo = m_CurUnitInfo;
	m_pAnimCon = GetOwner()->GetScript<CAnimController>();
}

void CUnitScript::tick()
{
	if (m_PrevUnitInfo.m_fHP > m_CurUnitInfo.m_fHP && m_CurUnitInfo.m_fHP != 0.f)
	{
		AddBit(m_CurUnitInfo.m_iStateBits, UNIT_HP_DOWN);
	}
	else if (m_CurUnitInfo.m_fHP == 0.f)
	{
		AddBit(m_CurUnitInfo.m_iStateBits, UNIT_HP_ZERO);
		m_CurUnitInfo.m_fHP = -1.f;
	}
	m_PrevUnitInfo = m_CurUnitInfo;
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
