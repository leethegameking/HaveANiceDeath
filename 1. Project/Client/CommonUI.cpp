#include "pch.h"
#include "CommonUI.h"

string CommonUI::popup_name = "";
bool CommonUI::close_bool = false;

void CommonUI::OpenPopup(const string& _name)
{
    ImGui::OpenPopup(_name.c_str());
    popup_name = _name;
}

void CommonUI::NotifyPopup()
{
    if (ImGui::BeginPopupModal(popup_name.c_str()))
    {
        if (CommonUI::ButtonCenteredOnLine("OK"))
        {
            ImGui::CloseCurrentPopup();
            close_bool = true;
        }
        else
        {
            close_bool = false;
        }
        ImGui::EndPopup();
    }

}


bool CommonUI::ButtonCenteredOnLine(const char* label, float alignment, float _size)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    if (size < _size)
    {
        size = _size;
    }

    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label, ImVec2(_size,0));
}

CommonUI::CommonUI()
{
}

CommonUI::~CommonUI()
{
}


