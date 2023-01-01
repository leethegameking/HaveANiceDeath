#include "pch.h"
#include "LayerNameTool.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

LayerNameTool::LayerNameTool()
	: UI("LayerNameTool")
{
	Close();
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_strIdx[i] = "(" + to_string(i) + ")";
	}
}

LayerNameTool::~LayerNameTool()
{
}


void LayerNameTool::update()
{
}

void LayerNameTool::render_update()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		ImGui::Text(m_strIdx[i].c_str());
		ImGui::SameLine();
		wstring wstrLayerName = pCurLevel->GetLayer(i)->GetName();
		string strLayerName = WstrToStr(wstrLayerName);
		string strTextName = "##TextLayerName" + to_string(i);

		static char buf[256];
		sprintf_s(buf, strLayerName.c_str());

		ImGui::InputText(strTextName.c_str(), buf, 256, ImGuiInputTextFlags_EnterReturnsTrue);

		if (KEY_TAP(KEY::ENTER))
		{
			pCurLevel->GetLayer(i)->SetName(StrToWstr(buf));
			CLevelMgr::GetInst()->SetLayerName(i, StrToWstr(buf));
			CEventMgr::GetInst()->LevelChangFlagOn();
		}
	}
}


