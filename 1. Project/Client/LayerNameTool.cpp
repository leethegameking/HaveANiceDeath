#include "pch.h"
#include "LayerNameTool.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CCollisionMgr.h>

LayerNameTool::LayerNameTool()
	: UI("LayerNameTool")
	, m_bMode((bool)LAYER_UI_MODE::EDIT_MODE)
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
	// radio button mode
	if (ImGui::RadioButton("Edit Mode", m_bMode == (bool)LAYER_UI_MODE::EDIT_MODE))
	{
		m_bMode = (bool)LAYER_UI_MODE::EDIT_MODE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Col Mode", m_bMode == (bool)LAYER_UI_MODE::COL_MODE))
	{
		m_bMode = (bool)LAYER_UI_MODE::COL_MODE;	
	}

	if (m_bMode == (bool)LAYER_UI_MODE::EDIT_MODE)
	{
		EditName();
	}
	else if (m_bMode == (bool)LAYER_UI_MODE::COL_MODE)
	{
		SetCol();
	}
}

void LayerNameTool::EditName()
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

void LayerNameTool::SetCol()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	ImGui::Text("");
	static std::pair<int, int> out;
	static bool bCheck = false;
	out = { 0, 0 };

	WORD* pColMat = CCollisionMgr::GetInst()->GetCollisionMat();
	
	// 줄 맞추기용
	static bool bOnce = true;

	float CursorPosX = ImGui::GetCursorPos().x;
	for (int i = 0; i < MAX_LAYER / 2; ++i)
	{
		ImGui::SameLine();
		string strLayerName = m_strIdx[i] + WstrToStr(pCurLevel->GetLayer(i)->GetName());
		ImGui::Text(strLayerName.c_str());
		
		for (int j = 0; j < i; j++)
		{
			string id = to_string(i) + "|" + to_string(j);
			bool bCollision; 
			if (pColMat[j] & (1 << i))
			{
				bCollision = true;
			}
			else
			{
				bCollision = false;
			}

			ImGui::SameLine();

			if (bOnce)
			{
				ImGui::SetCursorPosX(150.f);
				bOnce = false;
			}

			if (ImGui::Selectable(id.c_str(), bCollision, 0, Vec2(30.f, 15.f)))
			{
				if (bCollision)
					bCheck = true;
				else
					bCheck = false;

				out = std::pair<int, int>(i, j);
			}
			
		}
		ImGui::NewLine();
		bOnce = true;
	}
	
	if (out != std::pair<int,int>{0, 0})
	{
		tEvent evn = {};
		evn.eType = EVENT_TYPE::ARRANGE_COLLISION_LAYER;
		evn.wParam = (DWORD_PTR)&out;
		evn.lParam = (DWORD_PTR)bCheck;
		CEventMgr::GetInst()->AddEvent(evn);
	}
}


