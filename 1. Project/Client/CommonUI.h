#pragma once

#include "imgui.h"

class CommonUI
{
private:
	static string popup_name;
	static bool close_bool;
	static DWORD_PTR  data;

public:
	static void OpenPopup(const string& _name);
	static void NotifyPopup();
	static bool GetCloseBool() { return close_bool; }
	static void SetCloseBool(bool _b) { close_bool = _b; }

	static void InputTextPopup(const string& _InputType, char* buf);

	static bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f, float _size = 0.f);

	static void ColorPicker(Vec4& color);

public:
	static void SetData(DWORD_PTR _data) { data = _data; }
	static DWORD_PTR GetData() { return data; }
	

public:
	CommonUI();
	~CommonUI();
};


