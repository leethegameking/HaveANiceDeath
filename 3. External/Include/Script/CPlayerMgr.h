#pragma once

class CGameObject;

class CPlayerMgr
	: public CSingleton<CPlayerMgr>
{
private:
	CGameObject* m_pPlayerObj;
	CGameObject* m_pBossObj;

	bool		m_bPlayerDisable;

	tUnitInfo	m_pPlayerInfo;

public:
	CGameObject* GetPlayerObj() { return m_pPlayerObj; };
	void SetPlayerObj(CGameObject* _obj);
	bool GetPlayerDisable() { return m_bPlayerDisable; }
	void SetPlayerDisable(bool _b) { m_bPlayerDisable = _b; }

	void SavePlayerInfo();


	CGameObject* GetBossObj() { return m_pBossObj; }
	void SetBossObj(CGameObject* _obj) { m_pBossObj = _obj; }
	

	SINGLETON(CPlayerMgr)
};

