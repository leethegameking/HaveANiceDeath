#include "pch.h"
#include "CPlayerMgr.h"

#include "CUnitScript.h"

CPlayerMgr::CPlayerMgr()
	: m_pPlayerObj(nullptr)
	, m_pBossObj(nullptr)
	, m_bPlayerDisable(true)
{

}

CPlayerMgr::~CPlayerMgr()
{

}

void CPlayerMgr::SetPlayerObj(CGameObject* _obj)
{
	if (m_pPlayerObj != nullptr)
	{
		tUnitInfo& pCurInfo = _obj->GetScript<CUnitScript>()->GetUnitInfo();
		pCurInfo.m_fMaxHP = m_pPlayerInfo.m_fMaxHP;
		pCurInfo.m_fHP = m_pPlayerInfo.m_fHP;
	}

	m_pPlayerObj = _obj;
}

void CPlayerMgr::SavePlayerInfo()
{
	m_pPlayerInfo = m_pPlayerObj->GetScript<CUnitScript>()->GetUnitInfo();
}
