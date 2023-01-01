#pragma once

class CLevel;
class CLayer;
class CGameObject;

class CSaveLoadMgr
	: public CSingleton<CSaveLoadMgr>
{
SINGLETON(CSaveLoadMgr)

public:
	static bool m_bPrefabLoad;

private:
	static bool m_bFirstLoad;


public:
	void init();
	static bool IsPrefLoad() { return m_bPrefabLoad; }
	static void SetPrefLoad(bool _b) { m_bPrefabLoad = _b; }

public:
	static void SaveLevel(CLevel* _Level, wstring _strRelativePath);
	static CLevel* LoadLevel(wstring _strRelativePath);

public:
	static void SaveGameObject(CGameObject* _Object, FILE* _File);
	static CGameObject* LoadGameObject(FILE* _File);
};

