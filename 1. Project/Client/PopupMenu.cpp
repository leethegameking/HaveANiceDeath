#include "pch.h"
#include "PopupMenu.h"

#include "CommonUI.h"
#include "TreeUI.h"
#include <Engine/CRes.h>
#include <Engine/CGameObject.h>
#include <Engine/CEntity.h>


PopupMenuUI::PopupMenuUI(const string& _name)
	: UI(_name)
	, m_iMenuBit(0)
	, m_bHasAnyMenu(false)
{
}

PopupMenuUI::~PopupMenuUI()
{
}


void PopupMenuUI::update()
{
}

void PopupMenuUI::render_update()
{
	if (GetHasAnyMenu())
	{
		if (ImGui::BeginPopup(GetName().c_str()))
		{
			for (int i = 0; i < (UINT)MENU_TYPE::MENU_END; ++i)
			{
				if (m_iMenuBit & 1 << i)
				{
					if (ImGui::Selectable(Menu[i]))
					{
						CallMenuFunc(i, (DWORD_PTR)m_TreeUI->GetSelectedNode());
					}
				}
			}
			ImGui::EndPopup();
		}
	}


}



void PopupMenuUI::Open()
{
	ImGui::OpenPopup(GetName().c_str());
}

void PopupMenuUI::GetValidMenu(DWORD_PTR _node)
{
	m_iMenuBit = 0;
	m_bHasAnyMenu = false;

	TreeNode* pNode = (TreeNode*)_node;
	
	if (pNode == nullptr)
	{
		m_iMenuBit |= CREATE_EMPTY_OBJECT | ADD_NEW_ANIMATION | ADD_NEW_MATERIAL;
		m_bHasAnyMenu = true;
		return;
	}

	// Content
	CRes* pRes = dynamic_cast<CRes*>((CEntity*)pNode->GetData());

	if (pRes)
	{
		if(pRes->GetResType() == RES_TYPE::PREFAB)
		{
			m_iMenuBit |= INSTANTIATE | RENAME;
			m_bHasAnyMenu = true;
		}
		return;
	}

	// OutLiner
	CGameObject* pObj = dynamic_cast<CGameObject*>((CEntity*)pNode->GetData());

	if (pObj)
	{
		m_iMenuBit |= MAKE_PREFAB | DESTROY | RENAME;
		m_bHasAnyMenu = true;
		return;
	}

}

void PopupMenuUI::SetTreeUI(TreeUI* _TreeUI)
{
	m_TreeUI = _TreeUI;
}

