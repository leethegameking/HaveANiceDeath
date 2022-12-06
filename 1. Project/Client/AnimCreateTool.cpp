#include "pch.h"
#include "AnimCreateTool.h"

#include "imgui_internal.h"

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
	, m_iFrmIdx(0)
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
	Vec2 AtlasSize = Vec2(my_tex_w, my_tex_h);

	static Vec2 vRectStartPos = Vec2();
	static Vec2 vRectEndPos = Vec2();
	static int iMaxFrm = 1;
	static float fFPS = 0.f;
	static Vec2 fFullsize = Vec2(0.f, 0.f);
	static char strKey[255] = "";
	static float fPrevScrollX = 0;
	static float fPrevScrollY = 0;

	float fCurScrollX = ImGui::GetScrollX() - fPrevScrollX;
	float fCurScrollY = ImGui::GetScrollY() - fPrevScrollY;


	// 이미지 크기 표시
	ImGui::Text("Width :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_w) + "px"+ " |").c_str());
	ImGui::SameLine();
	ImGui::Text("Height :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_h) + "px").c_str());

	// 라디오버튼 
	if (ImGui::RadioButton("Uniformed Atlas", m_bUniformed == true)) { m_bUniformed = true; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Indulgent Atlas", m_bUniformed == false)) { m_bUniformed = false; }

	// 정렬 되었을 때 slice count 표시
	if (IsUniformed())
	{
		static int iSliceCount[2];
		iSliceCount[0] = (int)m_vSliceCount.x;
		iSliceCount[1] = (int)m_vSliceCount.y;
		ImGui::Text("SliceCount"); ImGui::SameLine();
		ImGui::InputInt2("##SliceCount", iSliceCount, ImGuiInputTextFlags_EnterReturnsTrue);
		m_vSliceCount = Vec2(iSliceCount[0], iSliceCount[1]);
	}




	// 내부 독립적인 창 생성
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove;
	ImGui::BeginChild("ChildImage", ImVec2(ImGui::GetContentRegionAvail().x, 500.f), true, window_flags);
	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	ImGui::Image(AtlasSRV, ImVec2(my_tex_w * m_vImageScale.x, my_tex_h * m_vImageScale.y),
		ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.f, 1.f, 1.f, 0.5f));

	// 이미지 클릭 이벤트 -> Idx반환
	if (m_bUniformed)
	{
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
		{
			m_vClickedIdx = GetClickedIdx(vCursorPos);
			m_bHasSelected = true;
		}
		
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		Vec2 vMousePos = io.MousePos;

		static float fPrevScrollX_Child = 0;
		static float fPrevScrollY_Child = 0;

		float fCurScrollX_Child = ImGui::GetScrollX() - fPrevScrollX_Child;
		float fCurScrollY_Child = ImGui::GetScrollY() - fPrevScrollY_Child;

		if (vMousePos.x < vCursorPos.x)
			vMousePos.x = vCursorPos.x;
		if (vMousePos.x > vCursorPos.x + AtlasSize.x * m_vImageScale.x)
			vMousePos.x = vCursorPos.x + AtlasSize.x * m_vImageScale.x;
		if (vMousePos.y < vCursorPos.y)
			vMousePos.y = vCursorPos.y;
		if (vMousePos.y > vCursorPos.y + AtlasSize.y * m_vImageScale.y)
			vMousePos.y = vCursorPos.y + AtlasSize.y * m_vImageScale.y;
		
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiID active_id = ImGui::GetActiveID();
		bool any_scrollbar_active = active_id && (active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_X) || active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y));

		// 그릴 사각형 좌표 잡기
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !any_scrollbar_active)
		{
			
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				vRectStartPos = vMousePos;
				vRectEndPos = vRectStartPos;
			}
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				vRectEndPos = vMousePos;
			}
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				vRectEndPos = vMousePos;
			}
		}

		Vec2 vCurScroll_Child = Vec2(fCurScrollX_Child, fCurScrollY_Child);

		vRectStartPos -= vCurScroll_Child;
		vRectEndPos -= vCurScroll_Child;

		Vec2 vCurScroll = Vec2(fCurScrollX, fCurScrollY);

		vRectStartPos -= vCurScroll;
		vRectEndPos -= vCurScroll;

		// 선택된 영역 네모 그리기
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRect(vRectStartPos, vRectEndPos, IM_COL32(0, 255, 0, 255));

		fPrevScrollX_Child = ImGui::GetScrollX();
		fPrevScrollY_Child = ImGui::GetScrollY();

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

		if (m_bHasSelected)
		{
			Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
			Vec2 StartPos = vCursorPos + m_vClickedIdx * vSlice * m_vImageScale;

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			if (IsVertical)
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x, vSlice.y * iMaxFrm) * m_vImageScale, IM_COL32(0, 255, 0, 255));
			else
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x * iMaxFrm, vSlice.y) * m_vImageScale, IM_COL32(0, 255, 0, 255));
		}

	}
	ImGui::EndChild();
	fPrevScrollX = ImGui::GetScrollX();
	fPrevScrollY = ImGui::GetScrollY();

	
	if (!m_bUniformed)
	{
		
		// UV로 계산
		Vec2 LTUV = (vRectStartPos - vCursorPos) / (AtlasSize * m_vImageScale);
		Vec2 RBUV = (vRectEndPos - vCursorPos) / (AtlasSize * m_vImageScale);

		// Frame 추가
		if (ImGui::Button("Add Frame"))
		{
			// 마우스 드래그 위치 (RT -> LB 같은) 경우 예외처리
			if (LTUV.x > RBUV.x)
			{
				float tmp = LTUV.x;
				LTUV.x = RBUV.x;
				RBUV.x = tmp;
			}
			if (LTUV.y > RBUV.y)
			{
				float tmp = LTUV.y;
				LTUV.y = RBUV.y;
				RBUV.y = tmp;
			}

			tAnim2DFrm tFrm = {};
			tFrm.vLeftTop = LTUV;
			tFrm.vOffset = Vec2::Zero;
			tFrm.fDuration = 0.1f;
			tFrm.vFullSize = Vec2(400.f, 400.f) / AtlasSize;
			tFrm.vSlice = RBUV - LTUV;

			m_ChangeFrm.push_back(tFrm);
		}

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildImageArr", Vec2(ImGui::GetContentRegionAvail().x, 100.f + 22.f), true, window_flags);
		// 프레임 수 만큼 이미지버튼으로 표시
		static int FrmID = 0;
		for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
		{
			ImGui::PushID(FrmID);
			++FrmID;
			if (ImGui::ImageButton(AtlasSRV, Vec2(100.f, 100.f), m_ChangeFrm[i].vLeftTop, m_ChangeFrm[i].vLeftTop + m_ChangeFrm[i].vSlice))
			{
				m_iFrmIdx = i;

			}
			// 지우기 옵션
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				DeleteFrame(i);
			}
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::EndChild();
	}

	// 정렬된 아틀라스일때
	if (m_bUniformed)
	{
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

	m_ChangeFrm.clear();
	m_iFrmIdx = 0;
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

void AnimCreateTool::DeleteFrame(size_t& idx)
{
	vector<tAnim2DFrm>::iterator iter = m_ChangeFrm.begin();
	for (; iter != m_ChangeFrm.end(); ++iter)
	{
		if (&(*iter) == &(m_ChangeFrm[m_iFrmIdx]))
		{
			iter = m_ChangeFrm.erase(iter);
			if (iter != m_ChangeFrm.end())
			{
				--idx;
			}
			break;
		}
	}
}



