#include "pch.h"
#include "PopupMenu.h"

#include "TreeUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CRes.h>
#include <Engine/CPrefab.h>




#define CALL_FUNC(Func) Func##_FUNC(_node);
void PopupMenuUI::CallMenuFunc(int _idx, DWORD_PTR _node)
{
	switch ((MENU_TYPE)_idx)
	{
	case MENU_TYPE::MAKE_PREFAB:
		CALL_FUNC(MAKE_PREFAB)
		break;
	case MENU_TYPE::INSTANTIATE:
		CALL_FUNC(INSTANTIATE)
		break;



	default:
		break;
	}
}

void PopupMenuUI::MAKE_PREFAB_FUNC(DWORD_PTR _node)
{
	TreeNode* pNode = (TreeNode*)_node;
	CGameObject* pObj = (CGameObject*)pNode->GetData();

	CPrefab* pPrefab = new CPrefab(pObj);

	CResMgr::GetInst()->AddRes<CPrefab>(pObj->GetName(), pPrefab);
}

void PopupMenuUI::INSTANTIATE_FUNC(DWORD_PTR _node)
{
	TreeNode* pNode = (TreeNode*)_node;
	CPrefab* pPrefab = (CPrefab*)pNode->GetData();

	Instantiate(pPrefab->Instantiate(), Vec3(0.f, 0.f, 0.f));
}