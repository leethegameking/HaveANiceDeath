#pragma once

class CommonUI
{
private:
	static string popup_name;
	static bool close_bool;

public:
	static void OpenPopup(const string& _name);
	static void NotifyPopup();
	static bool GetCloseBool() { return close_bool; }
	static void SetCloseBool(bool _b) { close_bool = _b; }

	static bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f, float _size = 0.f);
	

public:
	CommonUI();
	~CommonUI();
};


