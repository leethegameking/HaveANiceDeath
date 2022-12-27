#include "pch.h"
#include "ComponentUI.h"
#include "imgui_internal.h"

#include <Engine/CComponent.h>
#include <Engine/CScript.h>
#include "ScriptUI.h"

ComponentUI::ComponentUI(const string& _UIName, COMPONENT_TYPE _eType)
	: UI(_UIName)
	, m_TargetObj(nullptr)
	, m_eType(_eType)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::render_update()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
    ImGui::Button(ToString(m_eType));
    ImGui::PopStyleColor(3);
    ImGui::PopID();

	bool ActiveValue = GetTarget()->GetComponent(m_eType)->IsActive();
	ImGui::SameLine(); ImGui::Checkbox("Active", &ActiveValue);
	GetTarget()->GetComponent(m_eType)->SetActive(ActiveValue);

    ImGui::SameLine();
    float x = ImGui::GetContentRegionMax().x;
    float fCursorY = ImGui::GetCursorPosY();
    ImGui::SetCursorPos(Vec2(x - 10.f, fCursorY));

    DeleteCompButton();
}

void ComponentUI::DeleteCompButton()
{
    if (ImGui::Button("##DeleteComponent", Vec2(10.f, 10.f)))
    {
        if (m_eType == COMPONENT_TYPE::SCRIPT)
        {
            CComponent* pComp = ((ScriptUI*)this)->GetTargetScript();
            GetTarget()->DeleteComponent(pComp);
            SetTarget(nullptr);
        }
        else
        {
            CComponent* pComp = GetTarget()->GetComponent(m_eType);
            GetTarget()->DeleteComponent(pComp);
            SetTarget(nullptr);
        }
        Close();
    }
}


