#include "pch.h"
#include "CTriggerMgr.h"

#include <Engine/CGameObject.h>

CTriggerMgr::CTriggerMgr()
	: m_arrTriggerObj{}
{
}

CTriggerMgr::~CTriggerMgr()
{
}

void CTriggerMgr::RegisterTrigger(CGameObject* _pObj, TRIGGER_TYPE _eType)
{
	m_arrTriggerObj[(int)_eType] = _pObj;
}

CGameObject* CTriggerMgr::GetTriggerObj(TRIGGER_TYPE _eType)
{
	return m_arrTriggerObj[(int)_eType];
}

void CTriggerMgr::DestroyTriggerObj(TRIGGER_TYPE _eType)
{
	if (m_arrTriggerObj[(int)_eType])
	{
		m_arrTriggerObj[(int)_eType]->Destroy();
		m_arrTriggerObj[(int)_eType] = nullptr;
	}
}

