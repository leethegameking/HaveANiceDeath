﻿#pragma once

class CLevel;
class CLayer;
class CGameObject;

class CSaveLoadMgr
	: public CSingleton<CSaveLoadMgr>
{
SINGLETON(CSaveLoadMgr)

private:

public:
	void init();

public:
	void SaveLevel(CLevel* _Level, wstring _strRelativePath);
	CLevel* LoadLevel(wstring _strRelativePath);

public:
	static void SaveGameObject(CGameObject* _Object, FILE* _File);
	static CGameObject* LoadGameObject(FILE* _File);


};

