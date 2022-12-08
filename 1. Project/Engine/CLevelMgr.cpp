#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"


CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_strName, _out);
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

void CLevelMgr::ChangeLevel(CLevel* _nextLevel)
{
	if (m_pCurLevel)
		delete m_pCurLevel;

	m_pCurLevel = _nextLevel;
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;

	m_pCurLevel->SetName(L"Level 0");
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
}

void CLevelMgr::finaltick()
{
	m_pCurLevel->finaltick();
}
