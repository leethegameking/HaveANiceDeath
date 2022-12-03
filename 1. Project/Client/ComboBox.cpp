#include "pch.h"
#include "ComboBox.h"

UINT ComboBox::ID = 0;

ComboBox::ComboBox()
    : UI("##ComboBox"+to_string(ID))
    , m_iCurItemIdx(0)
{
}

ComboBox::~ComboBox()
{
}

void ComboBox::update()
{
}


void ComboBox::init(vector<string> _itemList, int _idx)
{
    SetItem(_itemList);
    m_iCurItemIdx = _idx;
}

void ComboBox::render_update()
{
    int flags = 0;

    if (ImGui::BeginCombo(GetName().c_str(), m_vecItem[m_iCurItemIdx].c_str(), flags))
    {
        for (int i = 0; i < m_vecItem.size(); i++)
        {
            const bool is_selected = (m_iCurItemIdx == i);
            if (ImGui::Selectable(m_vecItem[i].c_str(), is_selected))
                m_iCurItemIdx = i;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (m_SelectedFunc && m_SelectedInst)
                {
                    (m_SelectedInst->*m_SelectedFunc)((DWORD_PTR)m_vecItem[i].c_str());
                }
            }
        }
        ImGui::EndCombo();
    }
}




