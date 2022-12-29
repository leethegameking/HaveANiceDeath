#include "pch.h"
#include "InputTextUI.h"

#include "CommonUI.h"
#include "TreeUI.h"
#include <Engine/CGameObject.h>

InputTextUI::InputTextUI()
	: UI("##InputTextUI")
{
	SetSize(Vec2(200.f, 80.f));
	Close();
}

InputTextUI::~InputTextUI()
{
}

void InputTextUI::SetName(string _name)
{
	UI::SetName(_name);
}

void InputTextUI::Open()
{
	UI::Open();

}

void InputTextUI::render_update()
{

	ImGui::Text(strInputType.c_str()); ImGui::SameLine(); ImGui::InputText("##InputText", (char*)strText.data(), 256, ImGuiInputTextFlags_EnterReturnsTrue);

	if (CommonUI::ButtonCenteredOnLine("OK"))
	{
		Close();
		if (m_CloseInst && m_CloseFunc)
		{
			(m_CloseInst->*m_CloseFunc)(data, (DWORD_PTR)strText.data());
		}
		if (m_Close_Inst && m_Close_Func)
		{
			(m_Close_Inst->*m_Close_Func)((DWORD_PTR)strText.data());
		}
		ImGui::CloseCurrentPopup();
	}
}


