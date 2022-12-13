#pragma once

class CGameObject;
class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;


public:
	void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out);
	CGameObject* FindObjectByName(const wstring& _strName);
	void ChangeLevel(CLevel* _nextLevel);

public:
	CLevel* GetCurLevel() { return m_pCurLevel; }


public:
	void init();
	void progress();
	
private:
	void ChangeLevelState(LEVEL_STATE _State);


public:
	CLevelMgr();
	~CLevelMgr();

	friend class CEventMgr;
};

