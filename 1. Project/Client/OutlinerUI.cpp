#include "pch.h"
#include "OutlinerUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>

#include "CEditor.h"
#include "CGameObjectEx.h"

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

OutlinerUI::OutlinerUI()
	: UI("Outliner")
	, m_LevelTree(nullptr)
	, m_EditorTree(nullptr)
{
	m_LevelTree = new TreeUI("##OutlinerLevelTree");

	m_LevelTree->SetSize(Vec2(0.f, 300.f));
	m_LevelTree->SetShowSeperator(true);
	m_LevelTree->SetDummyRoot(false);

	m_LevelTree->AddDynamic_Select(this, (FUNC_1)&OutlinerUI::SetObjectToInspector);
	m_LevelTree->AddDynamic_DragDrop(this, (FUNC_2)&OutlinerUI::AddChildObject);
	
	AddChild(m_LevelTree);

	m_EditorTree = new TreeUI("##OutlinerEditorTree");
	
	m_EditorTree->SetSize(Vec2(0.f, 300.f));
	m_EditorTree->SetShowSeperator(true);
	m_EditorTree->SetDummyRoot(false);

	AddChild(m_EditorTree);

	ResetLevel();
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::update()
{
	if (CEventMgr::GetInst()->IsLevelChanged())
		ResetLevel();
}

void OutlinerUI::render_update()
{
}


void OutlinerUI::ResetLevel()
{
	m_LevelTree->Clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	string LevelName = string(pCurLevel->GetName().begin(), pCurLevel->GetName().end());

	TreeNode* pRootNode = m_LevelTree->AddItem(nullptr, "", LevelName.c_str(), 0, true);

	// 트리에 오브젝트 부모 - 자식 구조로 모두 등록
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = pCurLevel->GetLayer(i)->GetParentObjects();

		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			// 재귀적 호출
			AddGameObjectToTree(m_LevelTree, pRootNode, vecObject[j]);
		}
	}

	m_EditorTree->Clear();

	TreeNode* pEditorRootNode = m_EditorTree->AddItem(nullptr, "", "Editor", 0, true);

	vector<CGameObjectEx*>& vecEditorObj = CEditor::GetInst()->GetEditorObject();

	for (size_t i = 0; i < vecEditorObj.size(); ++i)
	{
		AddGameObjectToTree(m_EditorTree ,pEditorRootNode, (CGameObject*)vecEditorObj[i]);
	}
}


void OutlinerUI::SetObjectToInspector(DWORD_PTR _obj)
{
	// _obj = TreeNode*
	TreeNode* pSelectedNode = (TreeNode*)_obj;
	CGameObject* pObject = (CGameObject*)pSelectedNode->GetData();

	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	Inspector->SetTargetObj(pObject);
}

void OutlinerUI::AddGameObjectToTree(TreeUI* _TreeUI ,TreeNode* _ParentNode, CGameObject* _Object)
{
	string strObjectName = WstrToStr(_Object->GetName());
	TreeNode* pCurNode = _TreeUI->AddItem(_ParentNode, strObjectName.c_str(), strObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildObject();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(_TreeUI, pCurNode, vecChild[i]);
	}
}

void OutlinerUI::AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject)
{
	TreeNode* pChildObject = (TreeNode*)_ChildObject;
	TreeNode* pParentObject = (TreeNode*)_ParentObject;

	tEvent evt = {};
	evt.eType = EVENT_TYPE::ADD_CHILD;
	evt.wParam = pChildObject->GetData();
	evt.lParam = pParentObject->GetData();
	CEventMgr::GetInst()->AddEvent(evt);
}


