#include "pch.h"
#include "CUnitScript.h"

#include "CAnimController.h"

CUnitScript::CUnitScript()
	: CScript((int)SCRIPT_TYPE::UNITSCRIPT)
	, m_CurUnitInfo{}
	, m_pAnimCon(nullptr)
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
	, m_pAnimCon(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "HP    ", &m_CurUnitInfo.m_fHP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MP    ", &m_CurUnitInfo.m_fMP);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att   ", &m_CurUnitInfo.m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Def   ", &m_CurUnitInfo.m_fDef);
	AddScriptParam(SCRIPT_PARAM::INT, "State ", &m_CurUnitInfo.m_iStateBits);
}

CUnitScript::CUnitScript(const CUnitScript& _origin)
	: CScript(_origin)
	, m_CurUnitInfo(_origin.m_CurUnitInfo)
	, m_PrevUnitInfo{}
	, m_pAnimCon(nullptr)
{
}

CUnitScript::~CUnitScript()
{
}



void CUnitScript::begin()
{
	m_PrevUnitInfo = m_CurUnitInfo;
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

	// 넉백 처리.
	if (m_tKnockBack.bOn)
	{
		KnockBackProgress();
	}
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

void CUnitScript::KnockBackProgress()
{
	static float fAccTime = 0.f;
	if (m_tKnockBack.fDurationTime <= fAccTime)
	{
		m_tKnockBack.bOn = false;
		fAccTime = 0.f;
	}
	else
	{
		fAccTime += DT;
		Transform()->AddRelativePos(Vec3(m_tKnockBack.fDist * DT / m_tKnockBack.fDurationTime *(int)m_tKnockBack.eKnockBackDir, 0.f, 0.f));
	}
}