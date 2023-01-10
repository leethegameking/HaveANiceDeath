#include "pch.h"
#include "PrefabUI.h"

PrefabUI::PrefabUI()
	: ResUI("##TextureUI", RES_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::update()
{
	ResUI::update();
}

void PrefabUI::render_update()
{
	ImGui::Text("Prefab Save" , Vec2(20.f, 20.f)); ImGui::SameLine();
	if (ImGui::Button("##PrefSave"))
	{
		Ptr<CPrefab> pPref = (CPrefab*)GetTarget().Get();
		pPref->Save(pPref->GetRelativePath());
	}

}


