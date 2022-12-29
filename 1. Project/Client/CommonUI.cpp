#include "pch.h"
#include "CommonUI.h"

DWORD_PTR CommonUI::data = 0;
string CommonUI::popup_name = "";
bool CommonUI::close_bool = false;

void CommonUI::OpenPopup(const string& _name)
{
    ImGui::OpenPopup(_name.c_str());
    popup_name = _name;
}

void CommonUI::NotifyPopup()
{
    Vec2 vMousePos = ImGui::GetMousePos();
    ImGui::SetNextWindowPos(vMousePos);

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


void CommonUI::InputTextPopup(const string& _InputType,  char* buf)
{
    if (ImGui::BeginPopupModal(popup_name.c_str()))
    {
        ImGui::Text(_InputType.c_str()); ImGui::SameLine(); ImGui::InputText("##InputText", buf, 256, ImGuiInputTextFlags_EnterReturnsTrue);

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

void CommonUI::ColorPicker(Vec4& _color)
{
    float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker3("##MyColor##0", (float*)&_color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker3("##MyColor##1", (float*)&_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
}

CommonUI::CommonUI()
{
}

CommonUI::~CommonUI()
{
}


