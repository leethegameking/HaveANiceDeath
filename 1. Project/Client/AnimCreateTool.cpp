#include "pch.h"
#include "AnimCreateTool.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CResMgr.h>

#include "ComboBox.h"

AnimCreateTool::AnimCreateTool()
	: UI("AnimCreateTool")
	, m_vImageScale(0.5f, 0.5f)
	, m_bUniformed(false)
	, m_vSliceCount(Vec2(10.f, 10.f))
	, m_bHasSelected(false)
{
	m_AtlasComboBox = new ComboBox;
	Close();
}

AnimCreateTool::~AnimCreateTool()
{
	if (m_AtlasComboBox)
		delete m_AtlasComboBox;
}



void AnimCreateTool::init()
{
	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
	m_AtlasTex = (CTexture*)iter->second.Get();

	m_vecRes.clear();
	for (; iter != mapRes.end(); ++iter)
	{
		m_vecRes.push_back(string(iter->first.begin(), iter->first.end()));
	}
	m_AtlasComboBox->AddSelectedFunc(this, (FUNC_1)&AnimCreateTool::SetAtlasTex);
	m_AtlasComboBox->init(m_vecRes, 0);
}

void AnimCreateTool::update()
{
}

void AnimCreateTool::render_update()
{
	ImGui::Text("Image     "); ImGui::SameLine();
	m_AtlasComboBox->render_update();

	// UI ImageScale
	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("##ImageScaleX", &m_vImageScale.x, 0.1f, 0.f, "% .1f");
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vImageScale.y, 0.1f, 0.f, "% .1f");
	ImGui::PopItemWidth();

	ImTextureID AtlasSRV = m_AtlasTex->GetSRV().Get();
	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();
	
	// 이미지 크기 표시
	ImGui::Text("Width :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_w) + "px"+ " |").c_str());
	ImGui::SameLine();
	ImGui::Text("Height :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_h) + "px").c_str());

	// 정렬 되었을 때 slice count 표시
	ImGui::Checkbox("Uniformed Image", &m_bUniformed);
	if (m_bUniformed)
	{
		static int iSliceCount[2];
		iSliceCount[0] = (int)m_vSliceCount.x;
		iSliceCount[1] = (int)m_vSliceCount.y;
		ImGui::Text("SliceCount"); ImGui::SameLine();
		ImGui::InputInt2("##SliceCount", iSliceCount, ImGuiInputTextFlags_EnterReturnsTrue);
		m_vSliceCount = Vec2(iSliceCount[0], iSliceCount[1]);
	}

	// 내부 독립적인 창 생성
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 500.f), true, window_flags);
	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	ImGui::Image(AtlasSRV, ImVec2(my_tex_w * m_vImageScale.x, my_tex_h * m_vImageScale.y),
		ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.f, 1.f, 1.f, 0.5f));

	// 이미지 클릭 이벤트 -> Idx반환
	if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
	{
		m_vClickedIdx = GetClickedIdx(vCursorPos);
		m_bHasSelected = true;
	}

	// 그리드 그리기
	if (m_bUniformed)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Vec2 vGridStep = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
		for (float x = 0.f; x < my_tex_w; x += vGridStep.x)
			draw_list->AddLine(ImVec2(vCursorPos.x + x * m_vImageScale.x, vCursorPos.y),
							   ImVec2(vCursorPos.x + x * m_vImageScale.x, vCursorPos.y + my_tex_h * m_vImageScale.y),
							   IM_COL32(200, 200, 200, 40));

		for (float y = 0.f; y < my_tex_h; y += vGridStep.y)
			draw_list->AddLine(ImVec2(vCursorPos.x, vCursorPos.y + y * m_vImageScale.y),
				ImVec2(vCursorPos.x + my_tex_w * m_vImageScale.x, vCursorPos.y + y * m_vImageScale.y),
				IM_COL32(200, 200, 200, 40));
	}
	ImGui::EndChild();

	// 정렬된 아틀라스일때
	if (m_bUniformed)
	{
		static int iMaxFrm = 1;
		static float fFPS = 0.f;
		static Vec2 fFullsize = Vec2(0.f, 0.f);
		static char strKey[255] = "";
		ImGui::Text("AnimationKey"); ImGui::SameLine(); ImGui::InputText("##AnimationKey", &strKey[0], 255);
		ImGui::Text("MaxFrm      "); ImGui::SameLine(); ImGui::InputInt("##MaxFrm", &iMaxFrm, 0, 0);
		ImGui::Text("FPS         "); ImGui::SameLine(); ImGui::InputFloat("##FPS", &fFPS, 0, 0);
		ImGui::Text("FullSize    "); ImGui::SameLine(); ImGui::InputFloat2("##FPS", fFullsize);
		if (ImGui::RadioButton("Horizontal", !IsVertical)) { IsVertical = HORIZONTAL; }
		ImGui::SameLine();
		if(ImGui::RadioButton("Vertical", IsVertical)) { IsVertical = VERTICAL; }
		
		// Animation 생성
		Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
		if (ButtonCenteredOnLine("Create Uniformed Animation", 0.5f))
		{
			
			Vec2 LT = m_vClickedIdx * vSlice;
			float fStep;
			if (IsVertical)
				fStep = vSlice.y;
			else // HORIZONTAL
				fStep = vSlice.x;

			// Animation Create
			CAnimation2D* pAnimation = new CAnimation2D;
			pAnimation->Create(StrToWstr(strKey), m_AtlasTex, LT, vSlice, fStep, iMaxFrm, fFPS, fFullsize, IsVertical);

			ImGui::OpenPopup("Created");
		}

		// 확인 팝업.
		if (ImGui::BeginPopupModal("Created"))
		{
			ImGui::Text("Animation Created!!!");
			if (ButtonCenteredOnLine("Close", 0.5f))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		//if (ImGui::BeginPopup("Confirmed"))
		//{
		//	ImGui::Text("Animation Created!!!");
		//	ImGui::EndPopup();
		//}

		// 선택 네모 그리기
		if (m_bHasSelected)
		{
			Vec2 StartPos = vCursorPos +  m_vClickedIdx * vSlice * m_vImageScale;
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			if (IsVertical)
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x, vSlice.y * iMaxFrm) * m_vImageScale, IM_COL32(0, 255, 0, 255));
			else
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x * iMaxFrm, vSlice.y) * m_vImageScale, IM_COL32(0, 255, 0, 255));
		}
	}


}

void AnimCreateTool::Close()
{
	m_bUniformed = false;
	m_bHasSelected = false;
	m_vecRes.clear();
	UI::Close();
}

Vec2 AnimCreateTool::GetClickedIdx(Vec2 _vCursorPos)
{
	ImGuiIO& io = ImGui::GetIO();

	// KeyMgr가 받아오는 좌표와 차이가 있음.
	Vec2 vMousePos = io.MousePos;

	Vec2 TexCoord = ((vMousePos - _vCursorPos) / m_vImageScale);

	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();

	Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
	Vec2 TexIdx = Vec2((TexCoord.x / vSlice.x), TexCoord.y / vSlice.y);
	TexIdx.x = floor(TexIdx.x);
	TexIdx.y = floor(TexIdx.y);

	return TexIdx;
}

void AnimCreateTool::SetAtlasTex(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = StrToWstr(strKey);

	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	m_AtlasTex = (CTexture*)(mapRes.find(wstrKey)->second.Get());
}

