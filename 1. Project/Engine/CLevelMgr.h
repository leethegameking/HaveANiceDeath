#pragma once

class CGameObject;
class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;

	wstring			m_LayerName[MAX_LAYER]; 

public:
	void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out);
	CGameObject* FindObjectByName(const wstring& _strName);
	void ChangeLevel(CLevel* _nextLevel);

public:
	CLevel* GetCurLevel() { return m_pCurLevel; }

	wstring GetLayerName(int _idx) { return m_LayerName[_idx]; }
	void	SetLayerName(int _idx, const wstring& _name);


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

