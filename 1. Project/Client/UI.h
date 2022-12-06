#pragma once

// #include "imgui.h"

class UI
{
private:
	string m_strName;

	vector<UI*> m_vecChildUI;
	UI* m_ParentUI;

	ImVec2 m_vSize;

	bool m_bOpen;

	bool m_bModal;
	bool m_bPopUp;
	bool m_bChildAutoSize;
	bool m_bShowSeperator;

	int m_iWindowFlag;


public:
	virtual void update();
	void render();
	virtual void render_update() = 0;

public:
	const string& GetName() { return m_strName; }
	UI* GetParent() { return m_ParentUI; }

	void SetSize(ImVec2 _size) { m_vSize = _size; }

	ImVec2 GetSize() { return m_vSize; }

	void AddChild(UI* _ChildUI);

	virtual void init() {};
	virtual void Open() { m_bOpen = true; }
	virtual void Close() { m_bOpen = false; }


	void SetModal(bool _modal) { m_bModal = _modal; }
	bool IsModal() { return m_bModal; }

	void SetPopUp(bool _popup) { m_bPopUp = _popup; }
	bool IsPopUp() { return m_bPopUp; }

	void SetShowSeperator(bool _show) { m_bShowSeperator = _show; }
	bool ShowSeperator() { return m_bShowSeperator; }

	void SetAutoSize() { m_bChildAutoSize = true; }

	void ResetFocus();

	void AddFlag(ImGuiWindowFlags_ _flag) { m_iWindowFlag |= _flag; }


public:
	UI(const string& _strName);
	virtual ~UI();
};

// class의 함수 포인터
typedef void (UI::* FUNC_0)(void);
typedef void (UI::* FUNC_1)(DWORD_PTR);
typedef void (UI::* FUNC_2)(DWORD_PTR, DWORD_PTR);

