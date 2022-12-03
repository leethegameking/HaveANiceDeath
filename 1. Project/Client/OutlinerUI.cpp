#include "pch.h"
#include "OutlinerUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

OutlinerUI::OutlinerUI()
	: UI("Outliner")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##OutlinerTree");
	AddChild(m_Tree);

	m_Tree->SetShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_Selected(this, (FUNC_1)&OutlinerUI::SetObjectToInspector);

	ResetLevel();
}

OutlinerUI::~OutlinerUI()
{
}
void OutlinerUI::update()
{
}

void OutlinerUI::render_update()
{
}


void OutlinerUI::ResetLevel()
{
	m_Tree->Clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	string LevelName = string(pCurLevel->GetName().begin(), pCurLevel->GetName().end());

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, LevelName.c_str(), 0, true);

	// 트리에 오브젝트 부모 - 자식 구조로 모두 등록
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = pCurLevel->GetLayer(i)->GetParentObjects();

		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			// 재귀적 호출
			AddGameObjectToTree(pRootNode, vecObject[j]);
		}
	}
}


void OutlinerUI::SetObjectToInspector(DWORD_PTR _obj)
{
	// _obj = TreeNode*
	TreeNode* pSelectedNode = (TreeNode*)_obj;
	CGameObject* pObject = (CGameObject*)pSelectedNode->GetData();

	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	Inspector->SetTarget(pObject);
}

void OutlinerUI::AddGameObjectToTree(TreeNode* _ParentNode, CGameObject* _Object)
{
	string strObjectName = string(_Object->GetName().begin(), _Object->GetName().end());
	TreeNode* pCurNode = m_Tree->AddItem(_ParentNode, strObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildObject();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(pCurNode, vecChild[i]);
	}
}

void OutlinerUI::AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject)
{
}


