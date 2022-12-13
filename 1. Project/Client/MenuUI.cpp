#include "pch.h"
#include "MenuUI.h"

#include <Engine/CEventMgr.h>

MenuUI::MenuUI()
	: UI("##MenuUI")
{
}

MenuUI::~MenuUI()
{

}

void MenuUI::render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Level"))
		{
			if(ImGui::MenuItem("Play"))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
				CEventMgr::GetInst()->AddEvent(evn);
			}

			if (ImGui::MenuItem("Pause"))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::PAUSE;
				CEventMgr::GetInst()->AddEvent(evn);
			}

			if (ImGui::MenuItem("Stop"))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::STOP;
				CEventMgr::GetInst()->AddEvent(evn);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::render_update()
{
}


