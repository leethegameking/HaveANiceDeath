﻿#pragma once



class UI;

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
private:
	map<string, UI*> m_mapUI;
	
	HANDLE m_NotifyHandle;

public:
	UI* FindUI(const string& _name);

public:
	void init(HWND _hwnd);
	void progress();

private:
	void clear();
	void CreateUI();
	void ObserveContent();

public:
	CImGuiMgr();
	~CImGuiMgr();
};

