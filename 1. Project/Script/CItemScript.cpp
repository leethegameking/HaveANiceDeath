#include "pch.h"
#include "CItemScript.h"

#include "CPlayerMgr.h"

CItemScript::CItemScript()
	: CScript((int)SCRIPT_TYPE::ITEMSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Transtion", &m_Transition);
}

CItemScript::CItemScript(const CItemScript& _origin)
	: CScript(_origin)
	, m_Transition(_origin.m_Transition)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Transtion", &m_Transition);
}

CItemScript::~CItemScript()
{
}

void CItemScript::begin()
{
	m_fDelayTime = 0.8f;
	m_fAccTime = 0.f;
	b = false;
}

void CItemScript::tick()
{
	if (b)
	{
		if (m_fAccTime >= m_fDelayTime)
		{
			static string strLevel1RelPath = "level\\Ending.lv";
			tEvent evn;
			evn.eType = EVENT_TYPE::CHANGE_LEVEL;
			evn.wParam = (DWORD_PTR)strLevel1RelPath.data();
			CEventMgr::GetInst()->AddEvent(evn);

			evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
			evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
			CEventMgr::GetInst()->AddEvent(evn);
		}
		else
		{
			m_fAccTime += DT;
		}
	}
}

void CItemScript::BeginOverlap(CCollider2D* _pOther)
{
	CPlayerMgr::GetInst()->SetPlayerDisable(true);
	if (m_Transition.Get())
	{
		Instantiate(m_Transition->Instantiate(), Vec3::Zero);
	}
	b = true;
}

void CItemScript::Overlap(CCollider2D* _pOther)
{

}

void CItemScript::EndOverlap(CCollider2D* _pOther)
{
}

void CItemScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	SaveResourceRef(m_Transition, _pFile);
}

void CItemScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadResourceRef(m_Transition, _pFile);
}


