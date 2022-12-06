#include "pch.h"
#include "ResUI.h"

ResUI::ResUI(const string& _UIName, RES_TYPE _eType)
	: UI(_UIName)
    , m_eType(_eType)
{

}

ResUI::~ResUI()
{
}

void ResUI::update()
{
    if (GetTarget().Get())
    {
        m_wstrKey = GetTarget()->GetKey();
    }

    UI::update();
}

void ResUI::render_update()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
    ImGui::Button(ToString(m_eType));
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    if (m_eType != GetTarget()->GetResType())
        assert(nullptr);
}


