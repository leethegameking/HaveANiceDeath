#pragma once
#include "UI.h"

class PopupMenuUI
	:	public UI
{
private:
	int m_iMenuBit;

	bool m_bHasAnyMenu;


public:
	void update() override;
	void render_update() override;

public:
	void Open() override;

public:
	void GetValidMenu(DWORD_PTR _node); 
	void CallMenuFunc(int _idx, DWORD_PTR _node);

public:
	int GetMenuBit() { return m_iMenuBit; }
	bool GetHasAnyMenu() { return m_bHasAnyMenu; }

public:
	void MAKE_PREFAB_FUNC(DWORD_PTR _node);
	void INSTANTIATE_FUNC(DWORD_PTR _node);

public:
	PopupMenuUI(const string& _name);
	~PopupMenuUI();
};

