#include "pch.h"
#include "PopupMenu.h"

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
	
	CRes* pRes = dynamic_cast<CRes*>((CEntity*)pNode->GetData());
	if (pRes)
	{
		if(pRes->GetResType() == RES_TYPE::PREFAB)
		{
			m_iMenuBit |= INSTANTIATE;
			m_bHasAnyMenu = true;
		}
	}

	CGameObject* pObj = dynamic_cast<CGameObject*>((CEntity*)pNode->GetData());
	if (pObj)
	{
		m_iMenuBit |= MAKE_PREFAB;
		m_bHasAnyMenu = true;
	}
}




