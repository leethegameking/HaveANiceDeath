#include "pch.h"
#include "ListUI.h"

UINT ListUI::ID = 0;

ListUI::ListUI()
	: UI("ListUI")
	, m_iSelectIdx(-1)
	, m_DBCInst(nullptr)
	, m_DBCFunc(nullptr)
	, m_bMultiSelect(false)
	, m_bIntialized(false)
	, m_ID(ID)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_bMultiSelectIdx.push_back(false);
	}

	Close();
	SetSize(ImVec2(200.f, 500.f));
	++ID;
}

ListUI::ListUI(const string& _name)
	: UI(_name)
	, m_iSelectIdx(-1)
	, m_DBCInst(nullptr)
	, m_DBCFunc(nullptr)
	, m_bMultiSelect(false)
	, m_bIntialized(false)
	, m_ID(ID)
{
	Close();
	
	++ID;
}

ListUI::~ListUI()
{
}



void ListUI::init(vector<string> _itemList, int _bitInitial)
{
	SetItemList(_itemList);

	for (int i = 0; i < _itemList.size(); ++i)
	{
		m_bMultiSelectIdx.push_back((_bitInitial >> (_itemList.size() - i - 1)) % 2);
	}
}

void ListUI::render_update()
{
	ImVec2 vRegion = {};


	if (IsPopUp() || IsModal())
	{
		vRegion = ImGui::GetContentRegionAvail();
		vRegion.x += 18.f;
		vRegion.y -= 20.f;

		ImVec2 vCurPos = ImGui::GetWindowPos();
		vCurPos.x -= 0.f;
		vCurPos.y += 24.f;

		ImGui::SetNextWindowPos(vCurPos);
	}

	string str = "##ListBox" + to_string(m_ID);
	if (ImGui::BeginListBox(str.c_str(), vRegion))
	{
		static size_t PrevItemListSize = 0;

		if(m_bMultiSelect)
		{
			for (size_t i = 0; i < m_ItemList.size(); ++i)
			{
				char buf[32];
				sprintf_s(buf, m_ItemList[i].c_str(), i);

				if (m_ItemList.size() != PrevItemListSize)
				{
					m_bMultiSelectIdx.clear();
					for (size_t i = 0; i < m_ItemList.size(); ++i)
					{
						m_bMultiSelectIdx.push_back(false);
					}
				}

				if (ImGui::Selectable(buf, m_bMultiSelectIdx[i]))
				{
					if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
						m_bMultiSelectIdx.assign(m_bMultiSelectIdx.size(), 0);
						//memset(m_bMultiSelectIdx., 0, sizeof(m_bMultiSelectIdx));
					m_bMultiSelectIdx[i] = m_bMultiSelectIdx[i] ^ 1; 
				}

				if (m_SelectInst && m_SelectFunc)
				{
					(m_SelectInst->*m_SelectFunc)();
				}
			}

			PrevItemListSize = m_ItemList.size();
		}
		else
		{ 
			for (size_t i = 0; i < m_ItemList.size(); ++i)
			{
				bool Selectable = (m_iSelectIdx == i);

				// i번째 아이템이 눌렸다면
				if (ImGui::Selectable(m_ItemList[i].c_str(), Selectable))
				{
					// 눌린 아이템 인덱스를 저장해준다. -> 다시 들어올 때도 true
					m_iSelectIdx = i;
				}

				// 초기에 눌려 있는 아이템 설정
				//if (Selectable)
				//{
				//	ImGui::SetItemDefaultFocus();
				//}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (m_DBCInst && m_DBCFunc)
					{
						(m_DBCInst->*m_DBCFunc)((DWORD_PTR)m_ItemList[i].c_str());
					}
					if (IsModal())
						Close();
				}
			}
		}
		ImGui::EndListBox();

		if (ButtonCenteredOnLine("Confirm", 0.5f))
		{
			if (m_ConfirmInst && m_ConfirmFunc)
			{
				(m_ConfirmInst->*m_ConfirmFunc)((DWORD_PTR)&m_bMultiSelectIdx);
			}
			if (IsPopUp() || IsModal()) 
				Close();
		}
	}
}

void ListUI::Close()
{
	UI::Close();
	m_ItemList.clear();

	m_iSelectIdx = 0;

	m_DBCInst = nullptr;
	m_DBCFunc = nullptr;

	m_SelectInst = nullptr;
	m_SelectFunc = nullptr;


	UI* m_ConfirmInst = nullptr;
	FUNC_1 m_ConfirmFunc = nullptr;

	m_bMultiSelectIdx.clear();
	m_bIntialized = false;

	ResetFocus();
}

void ListUI::SetItemList(const vector<string>& _inList)
{
	m_ItemList = _inList;
}

void ListUI::SetItemList(const vector<wstring>& _inList)
{
	for (size_t i = 0; i < _inList.size(); ++i)
	{
		m_ItemList.push_back(string(_inList[i].begin(), _inList[i].end()));
	}
}


