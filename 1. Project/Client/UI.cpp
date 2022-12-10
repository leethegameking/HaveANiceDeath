#include "pch.h"
#include "UI.h"

#include "ComponentUI.h"

UI::UI(const string& _strName)
	: m_strName(_strName)
	, m_ParentUI(nullptr)
	, m_bOpen(true)
	, m_bModal(false)
	, m_bPopUp(false)
	, m_bShowSeperator(true)
	, m_bChildAutoSize(false)
	, m_iWindowFlag(0)
{
}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);
}

// Imgui에서 생성된 창들중에 focus된 것이 없도록 한다.
void UI::ResetFocus()
{
	ImGui::SetWindowFocus(nullptr); 
}

void UI::update()
{
	for (int i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void UI::render()
{
	if (m_bOpen)
	{
		// 부모 UI
		if (m_ParentUI == nullptr)
		{
			if (m_bModal)
			{
				// UI 생성되는 위치 설정( 화면 중앙 )
				ImVec2 vCenter = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(vCenter, ImGuiCond_::ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(m_vSize, ImGuiCond_::ImGuiCond_Appearing);

				// Modal Popup UI 띄우기
				ImGui::OpenPopup(m_strName.c_str());
				if (ImGui::BeginPopupModal(m_strName.c_str(), &m_bOpen))
				{
					render_update();

					for (int i = 0; i < m_vecChildUI.size(); ++i)
					{
						m_vecChildUI[i]->render();
					}

					ImGui::EndPopup();
				}
			}

			else
			{
				ImGui::Begin(m_strName.c_str(), &m_bOpen, m_iWindowFlag);

				render_update();

				for (int i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->render();
				}

				last_render();

				ImGui::End();
			}
		}

		// 자식 UI라면
		else
		{
			int flag = 0;

			ImGui::BeginChild(m_strName.c_str(), m_vSize, flag);

			render_update();

			for (int i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
			}

			ImGui::EndChild();

			if (ShowSeperator())
				ImGui::Separator();
		}

		if (m_bOpen == false)
		{
			Close();
		}
	}

	
}

void UI::AddChild(UI* _ChildUI)
{
	_ChildUI->m_ParentUI = this;
	m_vecChildUI.push_back(_ChildUI);
}