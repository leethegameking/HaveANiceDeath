#include "pch.h"
#include "InputTextUI.h"

#include "CommonUI.h"
#include "TreeUI.h"
#include <Engine/CGameObject.h>

InputTextUI::InputTextUI()
	: UI("##InputTextUI")
{
	Close();
}

InputTextUI::~InputTextUI()
{
}

void InputTextUI::SetName(string _name)
{
	UI::SetName(_name);
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
		ImGui::CloseCurrentPopup();
	}
}


