#include "pch.h"
#include "Animation2DUI.h"

#include "CImGuiMgr.h"

#include <Engine/CResMgr.h>
#include <Engine/Ptr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimation2D.h>

#include "FrameTool.h"
#include "AnimTool.h"

#include "CommonUI.h"

Animation2DUI::Animation2DUI()
	: ResUI("##Animation2D", RES_TYPE::ANIMATION2D)
	, m_bAnimPlay(false)
	, m_bFullsizeCheck(false)
	, m_vImageScale(0.5f, 0.5f)
	, m_prevAnim(nullptr)
{

}

Animation2DUI::~Animation2DUI()
{
}

void Animation2DUI::init()
{
	m_vecFrm = ((CAnimation2D*)GetTarget().Get())->GetFrmVec();
}

void Animation2DUI::update()
{


	ResUI::update();
}

void Animation2DUI::render_update()
{
	ResUI::render_update();

	CAnimation2D* pAnim = (CAnimation2D*)(GetTarget().Get());
	Ptr<CTexture> pAtlas = pAnim->GetAtlas();

	m_wstrKey = GetTarget()->GetKey();
	
	string text = "Key : " + WstrToStr(m_wstrKey);
	ImGui::Text(text.c_str());

	text = "AtlasKey : " + WstrToStr(pAtlas->GetKey());
	ImGui::Text(text.c_str());
	
	ImGui::Text("ImageScale"); ImGui::SameLine(); ImGui::InputFloat2("##ImageScale", m_vImageScale, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::Checkbox("Play", &m_bAnimPlay); ImGui::SameLine(); ImGui::Checkbox("Fullsize-Check", &m_bFullsizeCheck);

	DrawAnimation();

	if (CommonUI::ButtonCenteredOnLine("Edit Animation", 0.5f))
	{
		AnimTool* pAnimTool = (AnimTool*)CImGuiMgr::GetInst()->FindUI("AnimTool");

		pAnimTool->init_edit(pAnim);
		pAnimTool->Open();
	}

	if (CommonUI::ButtonCenteredOnLine("Save as file"))
	{
		pAnim->Save(L"animation\\test.anim");
	}
}

void Animation2DUI::DrawAnimation()
{
	CAnimation2D* pAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(GetTarget()->GetKey()).Get();
	Ptr<CTexture> pAtlas = pAnim->GetAtlas();
	ImTextureID AtlasSRV = pAtlas->GetSRV().Get();
	static int curIdx = 0;

	if (pAnim != m_prevAnim)
		curIdx = 0;
	m_prevAnim = pAnim;

	// Animation
	Vec2 vDiff = (m_vecFrm->at(curIdx).vFullSize - m_vecFrm->at(curIdx).vSlice) / 2.f;

	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	Vec2 vAtlasSize = Vec2(pAtlas->GetWidth(), pAtlas->GetHeight());
	Vec2 vRealDiff = vDiff * vAtlasSize;
	Vec2 vRealFullsize = m_vecFrm->at(curIdx).vFullSize * vAtlasSize;
	Vec2 vRealSlice = m_vecFrm->at(curIdx).vSlice * vAtlasSize;


	// 
	if (m_bFullsizeCheck)
	{
		Vec2 vLT = m_vecFrm->at(curIdx).vLeftTop - m_vecFrm->at(curIdx).vOffset - vDiff;
		Vec2 vRB = vLT + m_vecFrm->at(curIdx).vFullSize;
		Vec2 ImageSize = m_vecFrm->at(curIdx).vFullSize * vAtlasSize * m_vImageScale;

		// draw_list->AddRectFilled(vCursorPos, vCursorPos + ImageSize, IM_COL32_WHITE);
	
		ImGui::Image(AtlasSRV, ImageSize, vLT, vRB);
		// Fullsize Area
		draw_list->AddRectFilled(
			vCursorPos,
			vCursorPos + Vec2(ImageSize.x, vRealDiff.y * ImageSize.y / vRealFullsize.y),
			IM_COL32_WHITE, 0.f, 0);

		draw_list->AddRectFilled(
			vCursorPos,
			vCursorPos + Vec2(vRealDiff.x * ImageSize.x / vRealFullsize.x, ImageSize.y),
			IM_COL32_WHITE, 0.f, 0);

		Vec2 vStartCursorPos = Vec2(vCursorPos.x, vCursorPos.y + (vRealDiff.y + vRealSlice.y) * ImageSize.y / vRealFullsize.y);
		draw_list->AddRectFilled(
			vStartCursorPos,
			vStartCursorPos + Vec2(ImageSize.x, vRealDiff.y * ImageSize.y / vRealFullsize.y),
			IM_COL32_WHITE, 0.f, 0);

		vStartCursorPos = Vec2(vCursorPos.x + (vRealDiff.x + vRealSlice.x) * ImageSize.x / vRealFullsize.y, vCursorPos.y);
		draw_list->AddRectFilled(
			vStartCursorPos,
			vStartCursorPos + Vec2(vRealDiff.x * ImageSize.x / vRealFullsize.x, ImageSize.y),
			IM_COL32_WHITE, 0.f, 0);
	}
	else
	{
		Vec2 vLT = m_vecFrm->at(curIdx).vLeftTop - m_vecFrm->at(curIdx).vOffset - vDiff;
		Vec2 vRB = vLT + m_vecFrm->at(curIdx).vFullSize;
		Vec2 ImageSize = vRealSlice * m_vImageScale;

		ImGui::Image(AtlasSRV, vRealSlice, m_vecFrm->at(curIdx).vLeftTop, m_vecFrm->at(curIdx).vLeftTop + m_vecFrm->at(curIdx).vSlice);
	}

	if (m_bAnimPlay)
	{
		static float fAccTime = 0.f;
		fAccTime += DT;

		if (fAccTime > m_vecFrm->at(curIdx).fDuration)
		{
			++curIdx;
			if (m_vecFrm->size() == curIdx)
				curIdx = 0;

			fAccTime = fAccTime - m_vecFrm->at(curIdx).fDuration;
		}
	}
}


