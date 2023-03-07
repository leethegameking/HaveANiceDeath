#include "pch.h"
#include "CEffectMgr.h"

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>

#include <Engine/CGameObject.h>

CEffectMgr::CEffectMgr()
{
	const map<wstring, Ptr<CRes>>& mapPref = CResMgr::GetInst()->GetResource(RES_TYPE::PREFAB);
	RegisterFX(mapPref.find(L"prefab\\Transition_End.pref")->second, FX_TYPE::TRANSITION_END);
	RegisterFX(mapPref.find(L"prefab\\Transition_Start.pref")->second, FX_TYPE::TRANSITION_START);
}

CEffectMgr::~CEffectMgr()
{

}

void CEffectMgr::init()
{

}

void CEffectMgr::RegisterFX(Ptr<CRes> _pPref, FX_TYPE _eType)
{
	m_arrFXPref[(int)_eType] = _pPref;
}

CGameObject* CEffectMgr::GetFXObj(FX_TYPE _eType)
{
	m_arrFXObj[(int)_eType] = ((CPrefab*)m_arrFXPref[(int)_eType].Get())->Instantiate();
	return m_arrFXObj[(int)_eType];
}

void CEffectMgr::DestroyFX(FX_TYPE _eType)
{
	m_arrFXObj[(int)_eType]->Destroy();
	m_arrFXObj[(int)_eType] = nullptr;
}
