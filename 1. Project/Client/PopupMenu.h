#pragma once
#include "UI.h"

class TreeUI;

enum class MENU_TYPE
{
	// Outliner
	MAKE_PREFAB,
	CREATE_EMPTY_OBJECT,

	// Content
	INSTANTIATE,
	ADD_NEW_MATERIAL,
	ADD_NEW_ANIMATION,

	// General
	DESTROY,
	RENAME,

	MENU_END
};

extern const char* Menu[(UINT)MENU_TYPE::MENU_END];

enum MENU_BIT
{
	// Outliner
	MAKE_PREFAB = 0x01,
	CREATE_EMPTY_OBJECT = 0x02,

	// Content
	INSTANTIATE = 0x04,
	ADD_NEW_MATERIAL = 0x08,
	ADD_NEW_ANIMATION = 0x10,

	// General
	DESTROY = 0x20,
	RENAME = 0x40,
};




class PopupMenuUI
	:	public UI
{
private:
	int m_iMenuBit;
	bool m_bHasAnyMenu;

	TreeUI* m_TreeUI;

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
	// Outliner
	void MAKE_PREFAB_FUNC(DWORD_PTR _node);
	void CREATE_EMPTY_OBJECT_FUNC();

	// Content
	void INSTANTIATE_FUNC(DWORD_PTR _node);
	void ADD_NEW_MATERIAL_FUNC();
	void ADD_NEW_ANIMATION_FUNC();

	// General
	void DESTROY_FUNC(DWORD_PTR _node);
	void RENAME_FUNC(DWORD_PTR _node);

public:
	void SetTreeUI(TreeUI* _TreeUI);
	
public:
	PopupMenuUI(const string& _name);
	~PopupMenuUI();
};

