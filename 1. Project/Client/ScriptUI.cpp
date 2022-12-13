#include "pch.h"
#include "ScriptUI.h"

#include <Script/CScriptMgr.h>

int ScriptUI::ID = 0;

ScriptUI::ScriptUI()
	: ComponentUI("##ScriptUI##" +  to_string(ID), COMPONENT_TYPE::SCRIPT)
	, m_pTargetScript(nullptr)
{
    ++ID;
}

ScriptUI::~ScriptUI()
{
}


void ScriptUI::render_update()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));

    wstring strScriptName = CScriptMgr::GetScriptName(m_pTargetScript);
    ImGui::Button(WstrToStr(strScriptName).c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}


