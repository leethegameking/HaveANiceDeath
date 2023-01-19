#pragma once

class CGameObject;

class CUIMgr
	: public CSingleton<CUIMgr>
{	
	SINGLETON(CUIMgr)

private:
	Ptr<CRes> m_arrUIPref[(int)UI_TYPE::END];
	CGameObject* m_arrUIObj[(int)UI_TYPE::END];

public:
	void init();

	void RegisterUI(Ptr<CRes> _pPref, UI_TYPE _eType);
	CGameObject* GetUIObj(UI_TYPE _eType);

	void DestroyUI(UI_TYPE _eType);
};

