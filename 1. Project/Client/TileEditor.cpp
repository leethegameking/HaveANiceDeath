#include "pch.h"
#include "TileEditor.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>

#include "ComboBox.h"
#include "CommonUI.h"

TileEditor::TileEditor()
	: UI("TileEditor")
	, m_vChangeImageScale(Vec2(0.5f, 0.5f))
	, m_vTileMapImageScale(Vec2(0.5f, 0.5f))
{
	Close();
}

TileEditor::~TileEditor()
{
}

void TileEditor::init()
{
	tTile tile = {};
	m_vecTileChange.assign(m_TileMapUI->m_vecTile->begin(), m_TileMapUI->m_vecTile->end());
}

void TileEditor::update()
{
	UI::update();
}

void TileEditor::render_update()
{
	if (m_TileMapUI->m_bTileChanged)
	{
		m_vecTileChange.assign(m_TileMapUI->m_vecTile->begin(), m_TileMapUI->m_vecTile->end());

		m_TileMapUI->m_vTileCount = m_TileMapUI->GetTarget()->TileMap()->GetTileCount();
		m_TileMapUI->m_bTileChanged = false;
	}

	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("##ImageScaleX", &m_vChangeImageScale.x, 0.1f);
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vChangeImageScale.y, 0.1f);
	ImGui::PopItemWidth();

	// 타일맵 윈도우
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove;
	ImGui::BeginChild("Child_TileEditorImage",Vec2(ImGui::GetContentRegionAvail().x * 0.7f, ImGui::GetContentRegionAvail().y - 40.f), true, window_flags);
	ImTextureID AtlasSRV =  m_TileMapUI->m_AtlasTex->GetSRV().Get();
	Vec2 AtlasSize = m_TileMapUI->m_AtlasTex->GetSize();
	Vec2 targetScale = Vec2(m_TileMapUI->GetTarget()->Transform()->GetRelativeScale().x, m_TileMapUI->GetTarget()->Transform()->GetRelativeScale().y);

	// 클릭하면 선택된 이미지로 타일 이미지를 바꿔줌.
	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	for (int i = 0; i < (int)m_TileMapUI->m_vTileCount.y; ++i)
	{
		for (int j = 0; j < (int)m_TileMapUI->m_vTileCount.x; ++j)
		{
			int idx = i * m_TileMapUI->m_vTileCount.x + j;

			//int id = i * m_TileMapUI->m_vTileCount.x + j;
			//string strID = "TileID##" + to_string(id);
			//ImGui::PushID(i);
			ImGui::SetCursorScreenPos(vCursorPos + Vec2(j* (targetScale / m_TileMapUI->m_vTileCount * m_vChangeImageScale).x, i*(targetScale / m_TileMapUI->m_vTileCount * m_vChangeImageScale).y));
			ImGui::Image(AtlasSRV, targetScale / m_TileMapUI->m_vTileCount * m_vChangeImageScale,
				m_vecTileChange[idx].vLeftTop,
				m_vecTileChange[idx].vLeftTop + m_TileMapUI->m_vSlice/ AtlasSize);
			if (ImGui::IsItemHovered() && KEY_PRESSED(KEY::LBTN))
			{
				m_vecTileChange[idx].vLeftTop = m_TileMapUI->m_SelectedTexUV;
			}
			//ImGui::PopID();
			if (j != m_TileMapUI->m_vTileCount.x - 1)
				ImGui::SameLine();
		}
	}
	ImGui::EndChild();

	// 옵션 윈도우
	ImGui::SameLine();
	ImGui::BeginChild("Child_TileEditorVar", Vec2(0.f, ImGui::GetContentRegionAvail().y - 40.f), true, window_flags);
	// Atlas select combo-box
	ImGui::Text("Image     "); ImGui::SameLine();

	m_TileMapUI->m_AtlasComboBox->render_update();

	// TileCount
	ImGui::PushItemWidth(200.f);
	int tileCount[2] = { (int)m_TileMapUI->m_vTileCount.x, (int)m_TileMapUI->m_vTileCount.y };
	ImGui::Text("TileCount "); ImGui::SameLine(); ImGui::InputInt2("##TileCount", tileCount);
	ImGui::PopItemWidth();

	// Slice
	Vec2 prevSlice = m_TileMapUI->m_vSlice;
	ImGui::PushItemWidth(100.f);
	ImGui::Text("Slice     "); ImGui::SameLine(); ImGui::InputFloat2("##TileSlice", m_TileMapUI->m_vSlice, "%0.1f");
	ImGui::PopItemWidth();

	// UI ImageScale
	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("##ImageScaleX", &m_vTileMapImageScale.x, 0.1f);
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vTileMapImageScale.y, 0.1f);
	ImGui::PopItemWidth();


	// Image
	Vec2 vCursorPos_ = ImGui::GetCursorScreenPos();

	float my_tex_w = m_TileMapUI->m_AtlasTex->GetWidth();
	float my_tex_h = m_TileMapUI->m_AtlasTex->GetHeight();

	ImGui::Image(AtlasSRV, Vec2(my_tex_w * m_vTileMapImageScale.x, my_tex_h * m_vTileMapImageScale.y));
	if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
	{
		m_TileMapUI->SelectImageTile(vCursorPos_, m_vTileMapImageScale);
	}

	Vec2 p = Vec2(vCursorPos_.x + m_TileMapUI->m_vSlice.x * m_TileMapUI->m_SelectedTexIdx.x * m_vTileMapImageScale.x,
		vCursorPos_.y + m_TileMapUI->m_vSlice.y * m_TileMapUI->m_SelectedTexIdx.y * m_vTileMapImageScale.y);
	ImGui::GetWindowDrawList()->AddRect(
		p,
		Vec2(p.x + my_tex_w * m_vTileMapImageScale.x / (m_TileMapUI->m_AtlasTex->GetWidth() / m_TileMapUI->m_vSlice.x)
			, p.y + my_tex_h * m_vTileMapImageScale.y / (m_TileMapUI->m_AtlasTex->GetHeight() / m_TileMapUI->m_vSlice.y)), IM_COL32(0, 255, 0, 255));




	ImGui::EndChild();

	// mark square to image selected
	

	// Confirm
	if (CommonUI::ButtonCenteredOnLine("Confirm", 0.5f))
	{
		CommonUI::OpenPopup("Tile Edited!");
	}

	if (CommonUI::GetCloseBool())
	{
		m_TileMapUI->m_vecTile->assign(m_vecTileChange.begin(), m_vecTileChange.end());
		m_TileMapUI->GetTarget()->TileMap()->DataChanged();
		// Close();
		CommonUI::SetCloseBool(false);
	}

	CommonUI::NotifyPopup();

	if ((int)m_TileMapUI->m_vTileCount.x != tileCount[0] || (int)m_TileMapUI->m_vTileCount.y != tileCount[1])
	{
		m_TileMapUI->GetTarget()->TileMap()->SetTileCount((UINT)tileCount[0], (UINT)tileCount[1]);
		m_TileMapUI->m_bTileChanged = true;
	}
	if (m_TileMapUI->m_vSlice != prevSlice)
	{
		m_TileMapUI->GetTarget()->TileMap()->DataChanged();
		m_TileMapUI->GetTarget()->TileMap()->SetSlice(m_TileMapUI->m_vSlice);
	}
}

void TileEditor::Close()
{
	UI::Close();

	m_vecTileChange.clear();
}


