#include "pch.h"
#include "CUIMgr.h"

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>

#include <Engine/CGameObject.h>

CUIMgr::CUIMgr()
{
	const map<wstring, Ptr<CRes>>& mapPref = CResMgr::GetInst()->GetResource(RES_TYPE::PREFAB);
	RegisterUI(mapPref.find(L"prefab\\UI_BossHP.pref")->second, UI_TYPE::UI_BOSS_HP);
	RegisterUI(mapPref.find(L"prefab\\UI_LevelSelect.pref")->second, UI_TYPE::UI_SELECT_LV);
	RegisterUI(mapPref.find(L"prefab\\UI_PlayerState.pref")->second, UI_TYPE::UI_PLAYER_STATE);
}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::init()
{

}

void CUIMgr::RegisterUI(Ptr<CRes> _pPref, UI_TYPE _eType)
{
	m_arrUIPref[(int)_eType] = _pPref;
}

CGameObject* CUIMgr::GetUIObj(UI_TYPE _eType)
{
	m_arrUIObj[(int)_eType] = ((CPrefab*)m_arrUIPref[(int)_eType].Get())->Instantiate();
	return m_arrUIObj[(int)_eType];
}

void CUIMgr::DestroyUI(UI_TYPE _eType)
{
	m_arrUIObj[(int)_eType]->Destroy();
	m_arrUIObj[(int)_eType] = nullptr;
}


