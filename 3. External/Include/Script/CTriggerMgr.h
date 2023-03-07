#pragma once

class CGameObject;

class CTriggerMgr
	: public CSingleton<CTriggerMgr>
{
	SINGLETON(CTriggerMgr);

private:
	CGameObject* m_arrTriggerObj[(int)TRIGGER_TYPE::END];

public:
	void RegisterTrigger(CGameObject* _pObj, TRIGGER_TYPE _eType);

	CGameObject* GetTriggerObj(TRIGGER_TYPE _eType);
	void DestroyTriggerObj(TRIGGER_TYPE _eType);
};

