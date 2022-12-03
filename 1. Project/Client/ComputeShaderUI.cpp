#include "pch.h"
#include "ComputeShaderUI.h"

ComputeShaderUI::ComputeShaderUI()
	: ResUI("##ShaderUI", RES_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}


void ComputeShaderUI::update()
{
	ResUI::update();
}

void ComputeShaderUI::render_update()
{
	ResUI::render_update();
	string text = string(m_wstrKey.begin(), m_wstrKey.end());
	ImGui::Text("Key : "); ImGui::SameLine(); ImGui::Text(text.c_str());
}

