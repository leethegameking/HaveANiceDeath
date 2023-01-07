#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript()
	: CScript((int)SCRIPT_TYPE::UNITSCRIPT)
	, m_UnitInfo{}
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "HP    ", &m_UnitInfo.m_fHP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MP    ", &m_UnitInfo.m_fMP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att   ", &m_UnitInfo.m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Def   ", &m_UnitInfo.m_fDef);
	AddScriptParam(SCRIPT_PARAM::INT,	"State ", &m_UnitInfo.m_iStateBits);
}

CUnitScript::CUnitScript(int _ScriptType)
	: CScript(_ScriptType)
	, m_UnitInfo{}
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "HP    ", &m_UnitInfo.m_fHP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MP    ", &m_UnitInfo.m_fMP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att   ", &m_UnitInfo.m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Def   ", &m_UnitInfo.m_fDef);
	AddScriptParam(SCRIPT_PARAM::INT, "State ", &m_UnitInfo.m_iStateBits);
}

CUnitScript::~CUnitScript()
{
}



void CUnitScript::begin()
{
}

void CUnitScript::tick()
{

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
