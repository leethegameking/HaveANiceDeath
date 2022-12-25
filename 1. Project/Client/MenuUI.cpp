#include "pch.h"
#include "MenuUI.h"

#include "CSaveLoadMgr.h"
#include <Engine/CEventMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

MenuUI::MenuUI()
	: UI("##MenuUI")
	, m_ePrevState(LEVEL_STATE::STOP)
{
}

MenuUI::~MenuUI()
{

}

void MenuUI::render()
{
	// Menu enable - disable
	LEVEL_STATE eState = CLevelMgr::GetInst()->GetCurLevel()->GetState();
	static bool bPlay = false;
	static bool bPause = false;
	static bool bStop = false;
	if (eState == LEVEL_STATE::PLAY)
	{
		bPlay = false;
		bPause = true;
		bStop = true;
	}
	else if (eState == LEVEL_STATE::PAUSE)
	{
		bPlay = true;
		bPause = false;
		bStop = true;
	}
	else if (eState == LEVEL_STATE::STOP)
	{
		bPlay = true;
		bPause = false;
		bStop = false;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save Level", nullptr, nullptr, bStop))
			{
				CLevel* pSaveLevel = CLevelMgr::GetInst()->GetCurLevel();
				CSaveLoadMgr::GetInst()->SaveLevel(pSaveLevel, L"level\\testLV.lv");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Level"))
		{
			if(ImGui::MenuItem("Play", nullptr, nullptr, bPlay))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
				CEventMgr::GetInst()->AddEvent(evn);

				if(m_ePrevState == LEVEL_STATE::STOP)
					CSaveLoadMgr::SaveLevel(CLevelMgr::GetInst()->GetCurLevel(), L"tmp\\tmpsave.lv");

				m_ePrevState = LEVEL_STATE::PLAY;
			}

			if (ImGui::MenuItem("Pause", nullptr, nullptr, bPause))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::PAUSE;
				CEventMgr::GetInst()->AddEvent(evn);

				m_ePrevState = LEVEL_STATE::PAUSE;
			}

			if (ImGui::MenuItem("Stop", nullptr, nullptr, bStop))
			{
				tEvent evn = {};
				evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
				evn.wParam = (DWORD_PTR)LEVEL_STATE::STOP;
				CEventMgr::GetInst()->AddEvent(evn);

				evn.eType = EVENT_TYPE::CHANGE_LEVEL;
				evn.wParam = DWORD_PTR("tmp\\tmpsave.lv");
				CEventMgr::GetInst()->AddEvent(evn);

				m_ePrevState = LEVEL_STATE::STOP;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	
}

void MenuUI::render_update()
{
}


