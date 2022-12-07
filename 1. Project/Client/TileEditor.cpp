#include "pch.h"
#include "TileEditor.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>

TileEditor::TileEditor()
	: UI("TileEditor")
	, m_vImageScale(Vec2(0.5f, 0.5f))
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
	ImGui::InputFloat("##ImageScaleX", &m_vImageScale.x, 0.1f);
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vImageScale.y, 0.1f);
	ImGui::PopItemWidth();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove;
	ImGui::BeginChild("Child_TileEditorImage",Vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 200.f), true, window_flags);
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
			ImGui::SetCursorScreenPos(vCursorPos + Vec2(j* (targetScale / m_TileMapUI->m_vTileCount * m_vImageScale).x, i*(targetScale / m_TileMapUI->m_vTileCount * m_vImageScale).y));
			ImGui::Image(AtlasSRV, targetScale / m_TileMapUI->m_vTileCount * m_vImageScale,
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

	if (ButtonCenteredOnLine("Confirm", 0.5f))
	{
		m_TileMapUI->m_vecTile->assign(m_vecTileChange.begin(), m_vecTileChange.end());
		m_TileMapUI->GetTarget()->TileMap()->DataChanged();
	}
}

void TileEditor::Close()
{
	UI::Close();

	m_vecTileChange.clear();
}


