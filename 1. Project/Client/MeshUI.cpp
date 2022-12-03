#include "pch.h"
#include "MeshUI.h"

MeshUI::MeshUI()
	: ResUI("##MeshUI", RES_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}


void MeshUI::update()
{
	ResUI::update();
}

void MeshUI::render_update()
{
	ResUI::render_update();
	string text = string(m_wstrKey.begin(), m_wstrKey.end());
	ImGui::Text("Key : "); ImGui::SameLine(); ImGui::Text(text.c_str());
}

