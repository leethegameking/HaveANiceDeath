#include "pch.h"
#include "MenuUI.h"

#include "CSaveLoadMgr.h"
#include "CImGuiMgr.h"
#include "InputTextUI.h"

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
	static bool bPlay	= false;
	static bool bPause	= false;
	static bool bStop	= false;
	static bool bSave	= false;
	if (eState == LEVEL_STATE::PLAY)
	{
		bPlay = false;
		bPause = true;
		bStop = true;
		bSave = false;
	}
	else if (eState == LEVEL_STATE::PAUSE)
	{
		bPlay = true;
		bPause = false;
		bStop = true;
		bSave = false;
	}
	else if (eState == LEVEL_STATE::STOP)
	{
		bPlay = true;
		bPause = false;
		bStop = false;
		bSave = true;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create New Level", nullptr, nullptr))
			{
				InputTextUI* pInTextUI =(InputTextUI*)CImGuiMgr::GetInst()->FindUI("##InputTextUI");
				pInTextUI->Open();
				pInTextUI->SetPopupType("Level Name");
				pInTextUI->AddDynamic_Close_(this, (FUNC_1)&MenuUI::CreateNewLevel);
			}

			if (ImGui::MenuItem("Save Level", nullptr, nullptr, bSave))
			{
				SaveCurLevel();
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

void MenuUI::CreateNewLevel(DWORD_PTR _LevelName)
{
	SaveCurLevel();

	CLevel* pLevel = new CLevel;
	wstring strLevelName = StrToWstr(((char*)_LevelName));
	pLevel->SetName(strLevelName);
	static wstring wstrRelativePath;
	static string strRelativePath;

	wstrRelativePath = L"level\\" + strLevelName + L".lv";
	strRelativePath = WstrToStr(wstrRelativePath);
	CSaveLoadMgr::GetInst()->SaveLevel(pLevel, wstrRelativePath);
	delete pLevel;

	tEvent evn = {};
	evn.eType = EVENT_TYPE::CHANGE_LEVEL;
	evn.wParam = DWORD_PTR(strRelativePath.data());

	CEventMgr::GetInst()->ResChangeFlagOn();
	CEventMgr::GetInst()->AddEvent(evn);
}

void MenuUI::SaveCurLevel()
{
	CLevel* pSaveLevel = CLevelMgr::GetInst()->GetCurLevel();
	wstring strLevelName = pSaveLevel->GetName();
	wstring wstrRelativePath = L"level\\" + strLevelName + L".lv";

	CSaveLoadMgr::GetInst()->SaveLevel(pSaveLevel, wstrRelativePath);
}


