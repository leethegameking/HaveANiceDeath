#include "pch.h"

bool ButtonCenteredOnLine(const char* label, float alignment, float _size)
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