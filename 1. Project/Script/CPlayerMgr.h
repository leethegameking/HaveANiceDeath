#pragma once

class CGameObject;

class CPlayerMgr
	: public CSingleton<CPlayerMgr>
{
private:
	CGameObject* m_pPlayerObj;
	CGameObject* m_pBossObj;

	bool		m_bPlayerDisable;

public:
	CGameObject* GetPlayerObj() { return m_pPlayerObj; };
	void SetPlayerObj(CGameObject* _obj) { m_pPlayerObj = _obj; }
	bool GetPlayerDisable() { return m_bPlayerDisable; }
	void SetPlayerDisable(bool _b) { m_bPlayerDisable = _b; }



	CGameObject* GetBossObj() { return m_pBossObj; }
	void SetBossObj(CGameObject* _obj) { m_pBossObj = _obj; }
	

	SINGLETON(CPlayerMgr)
};

