#pragma once

class CGameObject;

class CObjectManager
	: public CSingleton<CObjectManager>
{
	SINGLETON(CObjectManager)

private:
	vector<CGameObject*> m_vecAllObject;

public:
	void RegisterObject(CGameObject* _pObj);
	CGameObject* FindObjByName(wstring _name);
	void Clear();
};

