#include "pch.h"
#include "PopupMenu.h"

#include "TreeUI.h"
#include "CommonUI.h"
#include "CImGuiMgr.h"
#include "InputTextUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CRes.h>
#include <Engine/CPrefab.h>
#include <Engine/CMaterial.h>

#include <Engine/CTransform.h>


extern const char* Menu[(UINT)MENU_TYPE::MENU_END] =
{
	// Outliner
	"Make Prefab",
	"Create Empty-Object",

	// Content
	"Instantiate",
	"New Material",
	"New Animation",

	// General
	"Destroy",
	"Rename"
};

#define CALL_FUNC_0(Func) Func##_FUNC()
#define CALL_FUNC_1(Func) Func##_FUNC(_node)
void PopupMenuUI::CallMenuFunc(int _idx, DWORD_PTR _node)
{
	switch (MENU_TYPE(_idx))
	{
	case MENU_TYPE::MAKE_PREFAB:
		CALL_FUNC_1(MAKE_PREFAB);
		break;
	case MENU_TYPE::CREATE_EMPTY_OBJECT:
		CALL_FUNC_0(CREATE_EMPTY_OBJECT);
		break;
	case MENU_TYPE::INSTANTIATE:
		CALL_FUNC_1(INSTANTIATE);
		break;
	case MENU_TYPE::ADD_NEW_MATERIAL:
		CALL_FUNC_0(ADD_NEW_MATERIAL);
		break;
	case MENU_TYPE::ADD_NEW_ANIMATION:
		CALL_FUNC_0(ADD_NEW_ANIMATION);
		break;
	case MENU_TYPE::DESTROY:
		CALL_FUNC_1(DESTROY);
		break;
	case MENU_TYPE::RENAME:
		CALL_FUNC_1(RENAME);
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

void PopupMenuUI::CREATE_EMPTY_OBJECT_FUNC()
{
	CGameObject* emptyObj = new CGameObject;
	emptyObj->SetName(L"New Object");
	emptyObj->AddComponent(new CTransform);

	Instantiate(emptyObj, Vec3::Zero);
}

void PopupMenuUI::INSTANTIATE_FUNC(DWORD_PTR _node)
{
	TreeNode* pNode = (TreeNode*)_node;
	CPrefab* pPrefab = (CPrefab*)pNode->GetData();

	Instantiate(pPrefab->Instantiate(), Vec3(0.f, 0.f, 0.f));
}

void PopupMenuUI::ADD_NEW_MATERIAL_FUNC()
{
	Ptr<CMaterial> pMtrl = new CMaterial;
	wstring strKey = CResMgr::GetInst()->GetNewResName<CMaterial>();
	CResMgr::GetInst()->AddRes<CMaterial>(strKey, pMtrl.Get());

	CEventMgr::GetInst()->ResChangeFlagOn();
}

void PopupMenuUI::ADD_NEW_ANIMATION_FUNC()
{
}

void PopupMenuUI::DESTROY_FUNC(DWORD_PTR _node)
{
	TreeNode* pNode = (TreeNode*)_node;
	CGameObject* delObj = (CGameObject*)(pNode->GetData());
	delObj->Destroy();
}

void PopupMenuUI::RENAME_FUNC(DWORD_PTR _node)
{
	InputTextUI* pUI = (InputTextUI*)CImGuiMgr::GetInst()->FindUI("##InputTextUI");
	pUI->SetPopupType("Name : ");
	pUI->SetData(_node);
	pUI->SetName("Set Name");
	pUI->Open();
	pUI->AddDynamic_Close(this, (FUNC_2)(&PopupMenuUI::Rename_Func_Pass));
}

void PopupMenuUI::Rename_Func_Pass(DWORD_PTR _node, DWORD_PTR _name)
{
	CGameObject* targetObj = (CGameObject*)(((TreeNode*)_node)->GetData());
	string name = (char*)_name;
	targetObj->SetName(StrToWstr(name));

	CEventMgr::GetInst()->LevelChangFlagOn();
}


