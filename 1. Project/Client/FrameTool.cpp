#include "pch.h"
#include "FrameTool.h"

#include "ComboBox.h"
#include "CommonUI.h"

#include <Engine/Ptr.h>

#include <Engine/CResMgr.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CTexture.h>



FrameTool::FrameTool()
	: UI("FrameTool")
	, m_iFrmIdx(FRAME_BUTTON)
	, m_bAnimChanged(false)
	, m_vImageScale(Vec2(0.5f, 0.5f))
{
	m_AnimComboBox = new ComboBox;
	Close();
}

FrameTool::~FrameTool()
{
	if (m_AnimComboBox)
		delete m_AnimComboBox;
}

void FrameTool::Init_Frame(CAnimation2D* _pAnim)
{
	m_pCurAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(_pAnim->GetKey()).Get();

	m_OriginFrm = m_pCurAnim->GetFrmVec();
	m_ChangeFrm.assign(m_OriginFrm->begin(), m_OriginFrm->end());

	const map<wstring, Ptr<CRes>>& mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapAnim.begin();

	int i = 0;
	int idx = 0;
	m_vecRes.clear();
	for (; iter != mapAnim.end(); ++iter)
	{
		m_vecRes.push_back(WstrToStr(iter->first));
		if (_pAnim == iter->second)
			idx = i;
		++i;
	}
	
	m_AnimComboBox->AddSelectedFunc(this, (FUNC_1)&FrameTool::SetAnimaton);
	m_AnimComboBox->init<CAnimation2D>(m_vecRes, idx);
}

void FrameTool::update()
{
	UI::update();
}

void FrameTool::render_update()
{
	// 애니메이션 선택
	m_AnimComboBox->render_update();
	Ptr<CTexture> pAtlas = m_pCurAnim->GetAtlas();
	ImTextureID AtlasSRV = pAtlas->GetSRV().Get();
	Vec2 vImageScale = Vec2(200.f, 200.f);


	if (m_bAnimChanged)
	{
		vector<tAnim2DFrm>* vecFrm = m_pCurAnim->GetFrmVec();
		m_ChangeFrm.assign(vecFrm->begin(), vecFrm->end());
		m_bAnimChanged = false;
	}

	if (ImGui::RadioButton("Button", m_opt == FRAME_BUTTON)) { m_opt = FRAME_BUTTON; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Image", m_opt == FRAME_IMAGE)) { m_opt = FRAME_IMAGE; }

	// 프레임 선택 [ 버튼 - 이미지 - 버튼 ]
	if (m_opt == FRAME_BUTTON)
	{
		if (ImGui::Button("Prev"))
		{
			--m_iFrmIdx;
			if (m_iFrmIdx < 0)
				m_iFrmIdx = m_ChangeFrm.size() - 1;
		}
		ImGui::SameLine();
		ImGui::Image(AtlasSRV, vImageScale * m_vImageScale, m_ChangeFrm[m_iFrmIdx].vLeftTop, m_ChangeFrm[m_iFrmIdx].vLeftTop + m_ChangeFrm[m_iFrmIdx].vSlice);
		ImGui::SameLine();
		if (ImGui::Button("Next"))
		{
			++m_iFrmIdx;
			if (m_ChangeFrm.size() <= m_iFrmIdx)
				m_iFrmIdx = 0;
		}
	}

	// 프레임 이미지로 배열
	if (m_opt == FRAME_IMAGE)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildFrameImage", ImVec2(ImGui::GetContentRegionAvail().x, vImageScale.y * m_vImageScale.y + 22.f), true, window_flags);

		for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
		{
			Vec2 vFrameIdxPos = ImGui::GetCursorScreenPos();
			string idx = "#imageButton" + to_string(i);
			ImGui::PushID(idx.c_str());
			if (ImGui::ImageButton(AtlasSRV, vImageScale* m_vImageScale, m_ChangeFrm[i].vLeftTop, m_ChangeFrm[i].vLeftTop + m_ChangeFrm[i].vSlice))
			{
				m_iFrmIdx = i;
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				m_iFrmIdx = i;
				DeleteFrame(m_iFrmIdx);
			}
			ImGui::PopID();
			string Frameidx = "[" + to_string(i) + "]";
			draw_list->AddText(vFrameIdxPos, IM_COL32_WHITE, Frameidx.c_str());
			ImGui::SameLine();
		}
		ImGui::Dummy(Vec2::Zero);

		ImGui::SameLine();
		static Ptr<CTexture> pPlusTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlusButton");
		ImTextureID plusID = pPlusTex->GetSRV().Get();
		if (ImGui::ImageButton(plusID, vImageScale * m_vImageScale))
		{
			m_ChangeFrm.push_back(tAnim2DFrm());
		}

		ImGui::EndChild();
	}

	
	// 프레임 정보
	if (!m_ChangeFrm.empty())
	{
		ImGui::PushItemWidth(100.f);
		ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", m_ChangeFrm[m_iFrmIdx].vLeftTop);
		ImGui::Text("Slice   ");	ImGui::SameLine(); ImGui::InputFloat2("##Slice", m_ChangeFrm[m_iFrmIdx].vSlice);
		ImGui::Text("Offset  ");	ImGui::SameLine(); ImGui::InputFloat2("##Offset", m_ChangeFrm[m_iFrmIdx].vOffset);
		ImGui::Text("Fullsize");	ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", m_ChangeFrm[m_iFrmIdx].vFullSize);
		ImGui::Text("Duration");	ImGui::SameLine(); ImGui::InputFloat("##Duation", &m_ChangeFrm[m_iFrmIdx].fDuration, 0, 0);
		ImGui::PopItemWidth();

	}
	// 컨펌
	if (CommonUI::ButtonCenteredOnLine("Confirm", 0.5f))
	{
		m_OriginFrm->assign(m_ChangeFrm.begin(), m_ChangeFrm.end());
		m_pCurAnim->Reallocate();
		ImGui::OpenPopup("Frame Edit");
	}
	if(ImGui::BeginPopupModal("Frame Edit"))
	{
		ImGui::Text("Frame Edit Complete!!!");
		if (CommonUI::ButtonCenteredOnLine("Close", 0.5f))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void FrameTool::Close()
{
	UI::Close();
}

void FrameTool::SetAnimaton(DWORD_PTR _animKey)
{
	string strAnimKey = (char*)_animKey;
	wstring wstrAnimKey = StrToWstr(strAnimKey);

	m_pCurAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(wstrAnimKey).Get();
	m_bAnimChanged = true;
}


void FrameTool::DeleteFrame(int& idx)
{
	vector<tAnim2DFrm>::iterator iter = m_ChangeFrm.begin();
	for (; iter != m_ChangeFrm.end(); ++iter)
	{
		if (&(*iter) == &(m_ChangeFrm[m_iFrmIdx]))
		{
			iter = m_ChangeFrm.erase(iter);
			if (iter != m_ChangeFrm.begin())
			{
				--idx;
			}
			break;
		}
	}
}