#include "pch.h"
#include "ScriptUI.h"

#include <Engine/CScript.h>
#include <Script/CScriptMgr.h>

#include "ParamUI.h"

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

    ImGui::SameLine();
    float x = ImGui::GetContentRegionMax().x;
    float fCursorY = ImGui::GetCursorPosY();
    ImGui::SetCursorPos(Vec2(x - 10.f, fCursorY));
    DeleteCompButton();

    render_scriptparam();
}

void ScriptUI::render_scriptparam()
{
    const vector<tScriptParam>& vecParam = m_pTargetScript->GetScriptParam();

    for (size_t i = 0; i < vecParam.size(); ++i)
    {
        switch (vecParam[i].Type)
        {
        case SCRIPT_PARAM::INT:
            ParamUI::Param_Int(vecParam[i].strParamName, (int*)(vecParam[i].pParam));
            break;
        case SCRIPT_PARAM::FLOAT:
            ParamUI::Param_Float(vecParam[i].strParamName, (float*)(vecParam[i].pParam));
            break;
        case SCRIPT_PARAM::VEC2:
            ParamUI::Param_Vec2(vecParam[i].strParamName, (Vec2*)(vecParam[i].pParam));
            break;
        case SCRIPT_PARAM::VEC3:
            ParamUI::Param_Vec3(vecParam[i].strParamName, (Vec3*)(vecParam[i].pParam));
            break;
        case SCRIPT_PARAM::VEC4:
            ParamUI::Param_Vec4(vecParam[i].strParamName, (Vec4*)(vecParam[i].pParam));
            break;
        case SCRIPT_PARAM::TEXTURE:
            break;
        case SCRIPT_PARAM::MATRERIAL:
            break;
        case SCRIPT_PARAM::PREFAB:
            break;
        case SCRIPT_PARAM::SOUND:
            break;
        default:
            break;
        }
    }
}


