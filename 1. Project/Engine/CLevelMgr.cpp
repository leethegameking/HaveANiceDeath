#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CRenderMgr.h"
#include "CObjectManager.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
	, m_LayerName{}
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

	CRenderMgr::GetInst()->ClearCam();
	CObjectManager::GetInst()->Clear();

	m_pCurLevel = _nextLevel;
}

void CLevelMgr::SetLayerName(int _idx, const wstring& _name)
{
	if (_name != "")
	{
		m_LayerName[_idx] = _name;
		// m_pCurLevel->GetLayer(_idx)->SetName(_name);
	}
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	// m_pCurLevel = nullptr;

	//m_pCurLevel->SetName(L"TestLevel");
}

void CLevelMgr::progress()
{
	m_pCurLevel->ClearLayer();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();
	}
	m_pCurLevel->finaltick();
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	if (m_pCurLevel->GetState() == _State)
		return;

	m_pCurLevel->SetState(_State);

	if (LEVEL_STATE::PLAY == _State)
		m_pCurLevel->begin();
}
