#include "pch.h"
#include "ComboBox.h"

UINT ComboBox::ID = 0;

ComboBox::ComboBox()
    : UI("##ComboBox"+to_string(ID))
    , m_iCurItemIdx(0)
    , m_SelectedFunc(nullptr)
    , m_SelectedInst(nullptr)
    , m_SelectedFunc_ReturnInt(nullptr)
    , m_SelectedInst_ReturnInt(nullptr)
    , m_SelectedNotifyFunc(nullptr)
    , m_SelectedNotifyInst(nullptr)
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

                if (m_SelectedFunc_ReturnInt && m_SelectedInst_ReturnInt)
                {
                    (m_SelectedInst_ReturnInt->*m_SelectedFunc_ReturnInt)((DWORD_PTR)i);
                }

                if (m_SelectedNotifyFunc && m_SelectedNotifyInst)
                {
                    (m_SelectedNotifyInst->*m_SelectedNotifyFunc)();
                }
            }
        }
        ImGui::EndCombo();
    }
}




