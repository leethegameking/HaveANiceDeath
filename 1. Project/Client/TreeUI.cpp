#include "pch.h"
#include "TreeUI.h"


#include "PopupMenu.h"
// ========
// TreeNode
// ========

int TreeNode::ID = 0;

TreeNode::TreeNode()
	: m_ParentNode(nullptr)
	, m_data(0)
	, m_ID(ID)
{
	++ID;
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChildNode);
}


void TreeNode::render_update()
{
	int iFlag = 0;
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;
	if (m_vecChildNode.empty())
		iFlag |= ImGuiTreeNodeFlags_Leaf;

	string strName = m_strName;
	if (m_vecChildNode.empty() && m_bFrame)
		strName = "\t" + strName;
	strName += "##" + to_string(m_ID);

	if (ImGui::TreeNodeEx(strName.c_str(), iFlag))
	{
		// ������ Ŭ�� üũ
		if (ImGui::IsItemHovered(0) && ImGui::IsMouseReleased(0) && !m_bFrame)
		{
			m_TreeUI->SetSelectedNode(this);
		}

		// ������ �巡�� üũ
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			m_TreeUI->SetBeginDragNode(this);

			// ImGui�� UI -> UI�� ������ �����ϴ� ��� : �̸�, void* Ÿ�� ������, ũ��
			ImGui::SetDragDropPayload(m_TreeUI->GetName().c_str(), (void*)this, sizeof(TreeNode));

			// drag & drop item name �Ľ� �߰�
			string strSubName;
			for (size_t i = 0; strName.length(); ++i)
			{
				if (strName[i] == '#')
				{
					strSubName = strName.substr(0, i);
					break;
				}
			}
			ImGui::Text(strSubName.c_str());
			

			ImGui::EndDragDropSource();
		}

		// ��� üũ
		if (ImGui::BeginDragDropTarget())
		{
			m_TreeUI->SetDropTargetNode(this);

			ImGui::EndDragDropTarget();
		}

		// ��Ŭ�� �̺�Ʈ
		if (ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			m_TreeUI->SetRightClickNode(this);
		}


		PopupMenuUI* pPopupMenu = m_TreeUI->GetPopupMenu();
		if (pPopupMenu->GetHasAnyMenu())
		{
			if (ImGui::BeginPopup(pPopupMenu->GetName().c_str()))
			{
				for (int i = 0; i < (UINT)MENU_TYPE::MENU_END; ++i)
				{
					int iMenuBit = pPopupMenu->GetMenuBit();

					if (iMenuBit & 1 << i)
					{
						if (ImGui::Selectable(Menu[i]))
						{
							pPopupMenu->CallMenuFunc(i, (DWORD_PTR)this);
						}
					}
				}
				ImGui::EndPopup();
			}
		}


		//	ImGui::EndPopup();
		//}
		//static int selected_fish = -1;
		//const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
		//static bool toggles[] = { true, false, false, false, false };

		//if (ImGui::Button("Select.."))
		//	ImGui::OpenPopup("my_select_popup");
		//ImGui::SameLine();
		//ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
		//if (ImGui::BeginPopup("my_select_popup"))
		//{
		//	ImGui::Text("Aquarium");
		//	ImGui::Separator();
		//	for (int i = 0; i < IM_ARRAYSIZE(names); i++)
		//		if (ImGui::Selectable(names[i]))
		//			selected_fish = i;
		//	ImGui::EndPopup();
		//}








		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}


// ======
// TreeUI
// ======

TreeUI::TreeUI(const string& _strName)
	: UI(_strName)
	, m_RootNode(nullptr)
	, m_SelectedNode(nullptr)
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
	, m_BeginDragNode(nullptr)
	, m_DropTargetNode(nullptr)

{
	m_PopupMenu = new PopupMenuUI("ContentMenu");
	AddDynamic_RightClick(m_PopupMenu, FUNC_1(&PopupMenuUI::GetValidMenu));
}

TreeUI::~TreeUI()
{
	if (m_PopupMenu)
		delete m_PopupMenu;
	Clear();
}



void TreeUI::update()
{
}

void TreeUI::render_update()
{
	if (m_RootNode == nullptr)
		return;

	if (!m_bDummyRootUse)
	{
		m_RootNode->render_update();
	}
	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChild();
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_update();
		}
	}

	// ���콺 ���� ������ üũ
	if (ImGui::IsMouseReleased(0))
	{
		m_BeginDragNode = nullptr;
		m_DropTargetNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetData(_data);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;

	if (_parent == nullptr)
	{
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

	return pNode;
}

void TreeUI::Clear()
{
	if (m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	if (m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}

	m_SelectedNode = _SelectedNode;
	m_SelectedNode->m_bSelected = true;

	if (m_SelectInst && m_SelectFunc)
	{
		(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
	}
}

void TreeUI::SetRightClickNode(TreeNode* _node)
{
	if (m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}

	m_SelectedNode = _node;
	m_SelectedNode->m_bSelected = true;

	if (m_RightClickInst && m_RightClickFunc)
	{
		(m_RightClickInst->*m_RightClickFunc)((DWORD_PTR)m_SelectedNode);
	}

	m_PopupMenu->Open();
}

void TreeUI::SetDropTargetNode(TreeNode* _node)
{
	m_DropTargetNode = _node;

	if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(GetName().c_str()))
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_BeginDragNode, (DWORD_PTR)m_DropTargetNode);
		}
	}
}

PopupMenuUI* TreeUI::GetPopupMenu()
{
	return m_PopupMenu;
}
