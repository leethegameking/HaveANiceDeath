#include "pch.h"
#include "CTriggerScript.h"
#include "CPlayerMgr.h"
#include "CTriggerMgr.h"
#include "CSoundMgr.h"

CTriggerScript::CTriggerScript()
	: CScript((int)SCRIPT_TYPE::TRIGGERSCRIPT)
	, m_eTriggerType((int)TRIGGER_TYPE::END)
	, m_eTriggerCond((int)TRIGGER_COND::END)
	, m_pFunc_0(nullptr)
	, m_pFunc_1(nullptr)
	, m_pFunc_2(nullptr)
	, m_Param_1()
	, m_Param_2()
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type ", &m_eTriggerType);
	AddScriptParam(SCRIPT_PARAM::INT, "Cond ", &m_eTriggerCond);
}

CTriggerScript::CTriggerScript(const CTriggerScript& _origin)
	: CScript(_origin)
	, m_eTriggerType(_origin.m_eTriggerType)
	, m_eTriggerCond(_origin.m_eTriggerCond)
	, m_pFunc_0(nullptr)
	, m_pFunc_1(nullptr)
	, m_pFunc_2(nullptr)
	, m_Param_1()
	, m_Param_2()
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type ", &m_eTriggerType);
	AddScriptParam(SCRIPT_PARAM::INT, "Cond ", &m_eTriggerCond);
}

CTriggerScript::~CTriggerScript()
{
}

void CTriggerScript::begin()
{
	m_bFirst = true;
}

void CTriggerScript::tick()
{
	if (m_pFunc_0)
		(this->*m_pFunc_0)();

	else if (m_pFunc_1)
		(this->*m_pFunc_1)(m_Param_1);

	else if (m_pFunc_2)
		(this->*m_pFunc_2)(m_Param_1, m_Param_2);
}

void CTriggerScript::BeginOverlap(CCollider2D* _pOther)
{
	if (m_eTriggerType == (int)TRIGGER_COND::BEGIN_OVERLAP)
	{
		ProgressTrigger(_pOther);
	}
}

void CTriggerScript::Overlap(CCollider2D* _pOther)
{
	if (m_eTriggerType == (int)TRIGGER_COND::OVERLAP)
	{
		ProgressTrigger(_pOther);
	}
}

void CTriggerScript::EndOverlap(CCollider2D* _pOther)
{
	if (m_eTriggerType == (int)TRIGGER_COND::END_OVERLAP)
	{
		ProgressTrigger(_pOther);
	}
}

void CTriggerScript::ProgressTrigger(CCollider2D* _pOther)
{
	switch ((TRIGGER_TYPE)m_eTriggerType)
	{
	case TRIGGER_TYPE::PLAYER_GO:
	{		
		m_pFunc_0 = (FUNC_0)(&CTriggerScript::PlayerGo);
	}
		break;

	case TRIGGER_TYPE::PLAYER_STOP:
	{
		m_pFunc_0 = (FUNC_0)(&CTriggerScript::PlayerStop);
		CTriggerMgr::GetInst()->DestroyTriggerObj(TRIGGER_TYPE::PLAYER_GO);
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Intro_A.wav", 1, 0.5f);
	}
		break;

	case TRIGGER_TYPE::CHANGE_CAMERA_BOSS_TYPE:
		break;
	}

	CTriggerMgr::GetInst()->RegisterTrigger(GetOwner(), (TRIGGER_TYPE)m_eTriggerType);
}

void CTriggerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_eTriggerType, sizeof(int), 1, _pFile);
	fwrite(&m_eTriggerCond, sizeof(int), 1, _pFile);
}

void CTriggerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_eTriggerType, sizeof(int), 1, _pFile);
	fread(&m_eTriggerCond, sizeof(int), 1, _pFile);
}



