#include "pch.h"
#include "FrameTool.h"

#include "ComboBox.h"

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
	m_AnimComboBox->init(m_vecRes, idx);
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
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildFrameImage", ImVec2(ImGui::GetContentRegionAvail().x, vImageScale.y * m_vImageScale.y + 22.f), true, window_flags);
		if (!m_ChangeFrm.empty())
			ImGui::PushID("#imageButton##0");
			if (ImGui::ImageButton(AtlasSRV, vImageScale * m_vImageScale, m_ChangeFrm[0].vLeftTop, m_ChangeFrm[0].vLeftTop + m_ChangeFrm[0].vSlice))
			{
				m_iFrmIdx = 0;
			}
			ImGui::PopID();
		for (size_t i = 1; i < m_ChangeFrm.size(); ++i)
		{
			ImGui::SameLine();
			string idx = "#imageButton" + to_string(i);
			ImGui::PushID(idx.c_str());
			if (ImGui::ImageButton(AtlasSRV, vImageScale* m_vImageScale, m_ChangeFrm[i].vLeftTop, m_ChangeFrm[i].vLeftTop + m_ChangeFrm[i].vSlice))
			{
				m_iFrmIdx = i;
			}
			ImGui::PopID();
		}

		ImGui::SameLine();
		static Ptr<CTexture> pPlusTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlusButton");
		ImTextureID plusID = pPlusTex->GetSRV().Get();
		if (ImGui::ImageButton(plusID, vImageScale * m_vImageScale))
		{
			m_ChangeFrm.push_back(tAnim2DFrm());
		}

		ImGui::EndChild();
	}

	ImGui::PushItemWidth(100.f);
	// 프레임 정보
	ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", m_ChangeFrm[m_iFrmIdx].vLeftTop);
	ImGui::Text("Slice   ");	ImGui::SameLine(); ImGui::InputFloat2("##Slice", m_ChangeFrm[m_iFrmIdx].vSlice);
	ImGui::Text("Offset  ");	ImGui::SameLine(); ImGui::InputFloat2("##Offset", m_ChangeFrm[m_iFrmIdx].vOffset);
	ImGui::Text("Fullsize");	ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", m_ChangeFrm[m_iFrmIdx].vFullSize);
	ImGui::Text("Duration");	ImGui::SameLine(); ImGui::InputFloat("##Duation", &m_ChangeFrm[m_iFrmIdx].fDuration, 0, 0);
	ImGui::PopItemWidth();

	// 컨펌
	if (ButtonCenteredOnLine("Confirm", 0.5f))
	{
		m_OriginFrm->assign(m_ChangeFrm.begin(), m_ChangeFrm.end());
		m_pCurAnim->Reallocate();
		ImGui::OpenPopup("Frame Edit");
	}
	if(ImGui::BeginPopupModal("Frame Edit"))
	{
		ImGui::Text("Frame Edit Complete!!!");
		if (ButtonCenteredOnLine("Close", 0.5f))
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


