#include "pch.h"
#include "GraphicsShaderUI.h"

GraphicsShaderUI::GraphicsShaderUI()
	: ResUI("##GraphicsShaderUI", RES_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}

void GraphicsShaderUI::update()
{
	ResUI::update();
}

void GraphicsShaderUI::render_update()
{
	ResUI::render_update();
	string text = string(m_wstrKey.begin(), m_wstrKey.end());
	ImGui::Text("Key : "); ImGui::SameLine(); ImGui::Text(text.c_str());
}


