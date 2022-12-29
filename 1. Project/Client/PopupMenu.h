﻿#pragma once
#include "UI.h"

class TreeUI;

enum class MENU_TYPE
{
	// Outliner
	MAKE_PREFAB,
	CREATE_EMPTY_OBJECT,
	MAKE_PARENT_OBJECT,

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
	MAKE_PARENT_OBJECT = 0x04,

	// Content
	INSTANTIATE = 0x08,
	ADD_NEW_MATERIAL = 0x10,
	ADD_NEW_ANIMATION = 0x20,

	// General
	DESTROY = 0x40,
	RENAME = 0x80,
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
	void MAKE_PARENT_OBJECT_FUNC(DWORD_PTR _node);

	// Content
	void INSTANTIATE_FUNC(DWORD_PTR _node);
	void ADD_NEW_MATERIAL_FUNC();
	void ADD_NEW_ANIMATION_FUNC();

	// General
	void DESTROY_FUNC(DWORD_PTR _node);
	void RENAME_FUNC(DWORD_PTR _node);
	void Rename_Func_Obj(DWORD_PTR _node, DWORD_PTR _name);
	void Rename_Func_Res(DWORD_PTR _node, DWORD_PTR _name);

public:
	void SetTreeUI(TreeUI* _TreeUI);
	
private:
	bool IsObject(DWORD_PTR _node);

public:
	PopupMenuUI(const string& _name);
	~PopupMenuUI();
};

