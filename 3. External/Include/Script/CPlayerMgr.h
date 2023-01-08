#pragma once

class CGameObject;

class CPlayerMgr
	: public CSingleton<CPlayerMgr>
{
private:
	CGameObject* m_pPlayerObj;

public:
	CGameObject* GetPlayerObj() { return m_pPlayerObj; };
	void SetPlayerObj(CGameObject* _obj) { m_pPlayerObj = _obj; }

	SINGLETON(CPlayerMgr)
};

