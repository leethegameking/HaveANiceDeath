#include "pch.h"
#include "AnimTool.h"

#include "ComboBox.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CAnimation2D.h>


AnimTool::AnimTool()
	: UI("AnimTool")
	, m_iMode((int)ToolMode::CREATE)
	, m_CreateFrmIdx(0)
	, m_EditFrmIdx(0)
	, m_ChangeFrm()
	, m_vSliceCount(Vec2(10.f, 10.f))
	, m_bVertical(false)
	, m_bUniformed(true)
	, m_bHasSelected(false)
	, m_bDragMode(false)
	, m_vAtlasScale(0.5f, 0.5f)
	, m_bImageChanged(true)
	, m_iMaxFrm(1)
	, m_bAnimChanged(true)
{
	Close();
	m_CreateCombo = new ComboBox;
	m_EditCombo = new ComboBox;

	// 디폴트
	map<wstring, Ptr<CRes>> mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
	map<wstring, Ptr<CRes>>::iterator iter = mapAnim.begin();
	m_pCurAnim = (CAnimation2D*)(iter->second.Get());
}

AnimTool::~AnimTool()
{
	if (m_CreateCombo)
		delete m_CreateCombo;

	if (m_EditCombo)
		delete m_EditCombo;
}


void AnimTool::init()
{
	SetDefaultTexture();

	// Get Texture Key, Intial Index
	SetCreateCombo();
}

void AnimTool::init_edit(CAnimation2D* _pAnim)
{
	m_iMode = (UINT)ToolMode::EDIT;
	m_pCurAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(_pAnim->GetKey()).Get();

	vector<tAnim2DFrm>* vecFrm =  m_pCurAnim->GetFrmVec();
	m_ChangeFrm.assign(vecFrm->begin(), vecFrm->end());

	const map<wstring, Ptr<CRes>>& mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapAnim.begin();
	vector<string> vecRes;

	m_AtlasTex = m_pCurAnim->GetAtlas().Get();
	m_AtlasSRV = m_AtlasTex->GetSRV().Get();

	int i = 0;
	int idx = 0;
	for (; iter != mapAnim.end(); ++iter)
	{
		vecRes.push_back(WstrToStr(iter->first));
		if (_pAnim == iter->second)
			idx = i;
		++i;
	}
	m_EditCombo->AddSelectedFunc(this, (FUNC_1)&AnimTool::SetAnimaton);
	m_EditCombo->init(vecRes, idx);
}

void AnimTool::update()
{
}

void AnimTool::render_update()
{
	static int prevMode = m_iMode;
	// set mode
	ToolModeSet();

	// Create Mode
	if (m_iMode == (int)ToolMode::CREATE)
	{
		if (m_iMode != prevMode)
		{
			ClearFrm();
			init();
		}
		CreateMode();
	}
	if (m_iMode == (int)ToolMode::EDIT)
	{
		if (m_iMode != prevMode)
		{
			ClearFrm();
			init_edit(m_pCurAnim.Get());
		}
		EditMode();
	}

	prevMode = m_iMode;

}




void AnimTool::CreateMode()
{
	ImGui::SameLine();
	m_CreateCombo->render_update();

	FrameWindow();
	AtlasWindow();
	ImGui::SameLine();
	FrameImageWindow();
	SettingWindow();
}

void AnimTool::EditMode()
{
	if (m_bAnimChanged)
	{
		vector<tAnim2DFrm>* vecFrm = m_pCurAnim->GetFrmVec();
		m_ChangeFrm.assign(vecFrm->begin(), vecFrm->end());
		m_bAnimChanged = false;
	}
	ImGui::SameLine();
	m_EditCombo->render_update();
	FrameWindow();
	FrameImageWindow();
	SettingWindow();
}






void AnimTool::SetDefaultTexture()
{
	m_AtlasTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerRunAtlas");
	m_AtlasSRV = m_AtlasTex->GetSRV().Get();
}

void AnimTool::SetCreateCombo()
{
	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();

	int Texidx = 0;
	int i = 0;
	vector<string> vecRes;
	for (; iter != mapRes.end(); ++iter)
	{
		if (m_AtlasTex.Get() == iter->second)
		{
			Texidx = i;
		}
		vecRes.push_back(string(iter->first.begin(), iter->first.end()));
		++i;
	}
	m_CreateCombo->init(vecRes, Texidx);
	m_CreateCombo->AddSelectedNotify(this, (FUNC_0)&AnimTool::ClearFrm);
	m_CreateCombo->AddSelectedFunc(this, (FUNC_1)&AnimTool::SetAtlasTex);
}

void AnimTool::ToolModeSet()
{
	if (ImGui::RadioButton("CREATE", m_iMode == (int)ToolMode::CREATE))
	{
		m_iMode = (int)ToolMode::CREATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("EDIT", m_iMode == (int)ToolMode::EDIT))
	{
		m_iMode = (int)ToolMode::EDIT;
	}
}












void AnimTool::FrameWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ChildImageFrame", Vec2(ImGui::GetContentRegionAvail().x, 100.f + 25.f), true, window_flags);

	// 프레임 수 만큼 이미지버튼으로 표시
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
	{
		Vec2 vFrameIdxPos = ImGui::GetCursorScreenPos();
		string imageID = "#imageButton" + to_string(i);
		ImGui::PushID(imageID.c_str());
		if (ImGui::ImageButton(m_AtlasSRV, Vec2(100.f, 100.f), m_ChangeFrm[i].vLeftTop, m_ChangeFrm[i].vLeftTop + m_ChangeFrm[i].vSlice))
		{
			if(IsCreateMode())
				m_CreateFrmIdx = i;
			if (IsEditMode())
				m_EditFrmIdx = i;
		}
		// 지우기 옵션
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			if (IsCreateMode())
			{
				m_CreateFrmIdx = i;
				DeleteFrame(m_CreateFrmIdx);
			}
			if (IsEditMode())
			{
				m_EditFrmIdx = i;
				DeleteFrame(m_EditFrmIdx);
			}
		}
		ImGui::PopID();
		string idx = "[" + to_string(i) + "]";
		draw_list->AddText(vFrameIdxPos, IM_COL32_WHITE, idx.c_str());

		ImGui::SameLine();
	}
	ImGui::EndChild();
}

void AnimTool::AtlasWindow()
{
	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();
	Vec2 AtlasSize = Vec2(my_tex_w, my_tex_h);

	static Vec2 vRectStartPos = Vec2();
	static Vec2 vRectEndPos = Vec2();
	static int iMaxFrm = 1;
	static float fFPS = 0.f;
	static Vec2 fFullsize = Vec2(0.f, 0.f);

	// 이미지 크기 표시
	ImGui::Text("Width :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_w) + "px" + " |").c_str());
	ImGui::SameLine();
	ImGui::Text("Height :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_h) + "px").c_str());

	// 라디오버튼 
	if (ImGui::RadioButton("Uniformed Atlas", m_bUniformed == true)) { m_bUniformed = true; m_bHasSelected = false; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Indulgent Atlas", m_bUniformed == false)) { m_bUniformed = false; m_bHasSelected = false; }


	if (IsIndulgent())
	{
		if (ImGui::RadioButton("Drag", m_bDragMode == true)) { m_bDragMode = true; }
		ImGui::SameLine();
		if (ImGui::RadioButton("Auto", m_bDragMode == false)) { m_bDragMode = false; }
	}

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

	static float fScale = 0.5f;
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 2.f);
	ImGui::SliderFloat("##AtlasScaleSlider", &fScale, 0.1f, 5.f);
	ImGui::PopItemWidth();
	m_vAtlasScale = Vec2(fScale, fScale);

	// 내부 독립적인 창 생성해서 이미지 띄움.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavInputs;
	ImGui::BeginChild("ChildImage", ImVec2(ImGui::GetContentRegionAvail().x / 2.f, 500.f), true, window_flags);
	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	ImGui::Image(m_AtlasSRV, ImVec2(my_tex_w * m_vAtlasScale.x, my_tex_h * m_vAtlasScale.y),
		ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.f, 1.f, 1.f, 0.5f));

	// 이미지 클릭 이벤트 -> Idx반환
	if (IsUniformed())
	{
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
		{
			m_vClickedIdx = GetClickedIdx(vCursorPos);
			m_bHasSelected = true;
		}

	}

	// 이미지 클릭시 사각형 그리고 LT, RB 좌표 알아냄. + Cursor를 기준으로 
	if (IsIndulgent())
	{
		ImGuiIO& io = ImGui::GetIO();
		Vec2 vMousePos = io.MousePos;

		static Vec2 vPrevCursor = vCursorPos;

		if (vMousePos.x < vCursorPos.x)
			vMousePos.x = vCursorPos.x;
		if (vMousePos.x > vCursorPos.x + AtlasSize.x * m_vAtlasScale.x)
			vMousePos.x = vCursorPos.x + AtlasSize.x * m_vAtlasScale.x;
		if (vMousePos.y < vCursorPos.y)
			vMousePos.y = vCursorPos.y;
		if (vMousePos.y > vCursorPos.y + AtlasSize.y * m_vAtlasScale.y)
			vMousePos.y = vCursorPos.y + AtlasSize.y * m_vAtlasScale.y;

		Vec2 TexCoord = vMousePos - vCursorPos;

		if (m_bDragMode)
		{
			// 스크롤 Released에서만 호출됨 -> 임시 처리.
			static bool bScrollBool = 0;
			// 그릴 사각형 좌표 잡기
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !KEY_PRESSED(KEY::LSHIFT))
				{
					vRectStartPos = vCursorPos + TexCoord;
					vRectEndPos = vRectStartPos;
					bScrollBool = true;
					m_bHasSelected = true;

				}
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !KEY_PRESSED(KEY::LSHIFT))
				{
					vRectEndPos = vCursorPos + TexCoord;
					bScrollBool = true;
				}
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !KEY_PRESSED(KEY::LSHIFT) && bScrollBool)
				{
					vRectEndPos = vCursorPos + TexCoord;
					bScrollBool = false;
				}

				// 사각형 드래그로 움직이기
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && KEY_PRESSED(KEY::LSHIFT))
				{
					Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
					vRectStartPos += Vec2(vMouseDir.x, -vMouseDir.y);
					vRectEndPos += Vec2(vMouseDir.x, -vMouseDir.y);
				}

				// 사각형 키입력으로 움직이기
				static float speed = 20.f;
				if (KEY_PRESSED(KEY::UP) && KEY_PRESSED(KEY::LSHIFT))
				{
					vRectStartPos.y -= speed * DT;
					vRectEndPos.y -= speed * DT;
				}
				if (KEY_PRESSED(KEY::DOWN) && KEY_PRESSED(KEY::LSHIFT))
				{
					vRectStartPos.y += speed * DT;
					vRectEndPos.y += speed * DT;
				}
				if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::LSHIFT))
				{
					vRectStartPos.x -= speed * DT;
					vRectEndPos.x -= speed * DT;
				}
				if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LSHIFT))
				{
					vRectStartPos.x += speed * DT;
					vRectEndPos.x += speed * DT;
				}

			}
		}
		// 자동선택모드
		else
		{
			// 미리 생성
			static vector<vector<tBGRA>> inVec;
			if (m_bImageChanged)
			{
				inVec.clear();
				m_AtlasTex->GetPixelVector(inVec);
				m_bImageChanged = false;
			}

			// 자동선택 버튼
			if (ImGui::IsMouseDoubleClicked(0) && KEY_PRESSED(KEY::LSHIFT))
			{
				Vec2 AutoSelectPos = Vec2(TexCoord.x / m_vAtlasScale.x, TexCoord.y / m_vAtlasScale.y);
				while (true)
				{
					// 결과로 나온 픽셀 중간에서 다시 너비탐색 시작 
					Vec4 out = m_AtlasTex->WIdthSearch(inVec, AutoSelectPos);
					AutoSelectPos = Vec2(out.z + 5.f, out.y + (out.w - out.y) / 2.f);

					// 더 이상 픽셀을 찾지 못한다면 탈출
					if (out.w - out.y == 0.f)
						break;

					Vec2 LTUV = Vec2(out.x, out.y) / AtlasSize;
					Vec2 RBUV = Vec2(out.z, out.w) / AtlasSize;
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
				m_AtlasTex->CheckClear(inVec);
			}


			// 클릭으로 선택
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(0))
			{
				if (ImGui::IsMouseClicked(0))
					m_bHasSelected = true;

				Vec4 out = m_AtlasTex->WIdthSearch(inVec, TexCoord / m_vAtlasScale);
				m_AtlasTex->CheckClear(inVec);

				vRectStartPos = Vec2(vCursorPos.x + out.x * m_vAtlasScale.x, vCursorPos.y + out.y * m_vAtlasScale.y);
				vRectEndPos = Vec2(vCursorPos.x + out.z * m_vAtlasScale.x, vCursorPos.y + out.w * m_vAtlasScale.y);

				// 더블클릭으로 프레임 추가
				if (ImGui::IsMouseDoubleClicked(0) && !KEY_PRESSED(KEY::LSHIFT))
				{
					Vec2 LTUV = Vec2(out.x, out.y) / AtlasSize;
					Vec2 RBUV = Vec2(out.z, out.w) / AtlasSize;

					tAnim2DFrm tFrm = {};
					tFrm.vLeftTop = LTUV;
					tFrm.vOffset = Vec2::Zero;
					tFrm.fDuration = 0.1f;
					tFrm.vFullSize = Vec2(400.f, 400.f) / AtlasSize;
					tFrm.vSlice = RBUV - LTUV;

					m_ChangeFrm.push_back(tFrm);
				}
			}
		}

		Vec2 vChangedCursorPos = vCursorPos - vPrevCursor;

		vRectStartPos += vChangedCursorPos;
		vRectEndPos += vChangedCursorPos;

		vPrevCursor = vCursorPos;

		// 선택된 영역 네모 그리기
		if (m_bHasSelected)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRect(vRectStartPos, vRectEndPos, IM_COL32(0, 255, 0, 255));
		}
	}

	// 그리드  및 사각형 그리기
	if (IsUniformed())
	{
		// 그리드
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Vec2 vGridStep = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
		for (float x = 0.f; x < my_tex_w; x += vGridStep.x)
			draw_list->AddLine(ImVec2(vCursorPos.x + x * m_vAtlasScale.x, vCursorPos.y),
				ImVec2(vCursorPos.x + x * m_vAtlasScale.x, vCursorPos.y + my_tex_h * m_vAtlasScale.y),
				IM_COL32(200, 200, 200, 40));

		for (float y = 0.f; y < my_tex_h; y += vGridStep.y)
			draw_list->AddLine(ImVec2(vCursorPos.x, vCursorPos.y + y * m_vAtlasScale.y),
				ImVec2(vCursorPos.x + my_tex_w * m_vAtlasScale.x, vCursorPos.y + y * m_vAtlasScale.y),
				IM_COL32(200, 200, 200, 40));

		// 사각형
		if (m_bHasSelected)
		{
			Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
			Vec2 StartPos = vCursorPos + m_vClickedIdx * vSlice * m_vAtlasScale;

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			if (m_bVertical)
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x, vSlice.y * m_iMaxFrm) * m_vAtlasScale, IM_COL32(0, 255, 0, 255));
			else
				draw_list->AddRect(StartPos, StartPos + Vec2(vSlice.x * m_iMaxFrm, vSlice.y) * m_vAtlasScale, IM_COL32(0, 255, 0, 255));
		}
	}
	ImGui::EndChild();
}

void AnimTool::SettingWindow()
{
	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();
	Vec2 vAtlasSize = m_AtlasTex->GetSize();

	if (IsUniformed() && IsCreateMode())
	{
		static float fFPS = 1.f;
		static Vec2 vFullsize = Vec2(400.f, 400.f);
		static Vec2 vOffset = Vec2::Zero ;

		
		ImGui::Text("MaxFrm      "); ImGui::SameLine(); ImGui::InputInt("##MaxFrm", &m_iMaxFrm, 0, 0);
		ImGui::Text("FPS         "); ImGui::SameLine(); ImGui::InputFloat("##FPS", &fFPS, 0, 0);
		ImGui::Text("FullSize    "); ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", vFullsize);
		ImGui::Text("Offset      "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", vOffset);

		for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
		{
			m_ChangeFrm[i].fDuration = 1.f / fFPS;
			m_ChangeFrm[i].vFullSize = vFullsize / vAtlasSize;
			m_ChangeFrm[i].vOffset = vOffset / vAtlasSize;
		}

		if (ImGui::RadioButton("Horizontal", !m_bVertical)) { m_bVertical = HORIZONTAL; }
		ImGui::SameLine();
		if (ImGui::RadioButton("Vertical", m_bVertical)) { m_bVertical = VERTICAL; }

		Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
		Vec2 LT = m_vClickedIdx * vSlice;

		float fStep;
		if (m_bVertical)
			fStep = vSlice.y;
		else // HORIZONTAL
			fStep = vSlice.x;

		if (CommonUI::ButtonCenteredOnLine("Confirm", 0.5f))
		{
			ImGui::OpenPopup("New Animation Create");
		}

		if (ImGui::BeginPopupModal("New Animation Create"))
		{
			static char strKey[256] = "";
			ImGui::Text("AnimationKey :"); ImGui::SameLine(); ImGui::InputText("##AnimKey", strKey, 255, ImGuiInputTextFlags_EnterReturnsTrue);
			if (CommonUI::ButtonCenteredOnLine("Confirm", 0.3f, 60.f))
			{
				CResMgr::GetInst()->CreateAnimation(StrToWstr(strKey), m_AtlasTex, LT, vOffset, vSlice, fStep, m_iMaxFrm, fFPS, vFullsize, m_bVertical);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (CommonUI::ButtonCenteredOnLine("Close", 0.f, 60.f))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	if (IsIndulgent() || IsEditMode())
	{
		if (!m_ChangeFrm.empty())
		{
			if (ImGui::RadioButton("One by One", m_bAllChange == false)) { m_bAllChange = false; }
			ImGui::SameLine();
			if (ImGui::RadioButton("All", m_bAllChange == true)) { m_bAllChange = true; }

			int idx;
			if (IsCreateMode())
				idx = m_CreateFrmIdx;
			if (IsEditMode())
				idx = m_EditFrmIdx;

			if (m_bAllChange == false)
			{
				Vec2 vLeftTop = m_ChangeFrm[idx].vLeftTop * vAtlasSize;
				Vec2 vSlice = m_ChangeFrm[idx].vSlice * vAtlasSize;
				Vec2 vFullSize = m_ChangeFrm[idx].vFullSize * vAtlasSize;
				Vec2 vOffset = m_ChangeFrm[idx].vOffset * vAtlasSize;


				ImGui::PushItemWidth(100.f);
				// 프레임 정보
				ImGui::Text("LeftTop ");	ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", vLeftTop);
				ImGui::Text("Slice   ");	ImGui::SameLine(); ImGui::InputFloat2("##Slice", vSlice);
				ImGui::Text("Offset  ");	ImGui::SameLine(); ImGui::InputFloat2("##Offset", vOffset);
				ImGui::Text("Fullsize");	ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", vFullSize);
				ImGui::Text("Duration");	ImGui::SameLine(); ImGui::InputFloat("##Duation", &m_ChangeFrm[m_CreateFrmIdx].fDuration, 0, 0);
				ImGui::PopItemWidth();

				m_ChangeFrm[idx].vSlice = vSlice / vAtlasSize;
				m_ChangeFrm[idx].vOffset = vOffset / vAtlasSize;
				m_ChangeFrm[idx].vFullSize = vFullSize / vAtlasSize;
				m_ChangeFrm[idx].vLeftTop = vLeftTop / vAtlasSize;
			}
			if (m_bAllChange)
			{
				static float fFPS = 0.f;
				static Vec2 vFullsize = Vec2(400.f, 400.f);
				static Vec2 vOffset = Vec2::Zero;

				ImGui::Text("FPS         "); ImGui::SameLine(); ImGui::InputFloat("##FPS", &fFPS, 0, 0);
				ImGui::Text("FullSize    "); ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", vFullsize);
				ImGui::Text("Offset      "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", vOffset);


				for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
				{
					m_ChangeFrm[i].fDuration = 1.f / fFPS;
					m_ChangeFrm[i].vFullSize = vFullsize / vAtlasSize;
					m_ChangeFrm[i].vOffset = vOffset / vAtlasSize;

				}
			}
		}

		// 컨펌
		if (IsCreateMode())
		{
			if (CommonUI::ButtonCenteredOnLine("Confirm", 0.5f))
			{
				ImGui::OpenPopup("New Animation Create");
			}
			if (ImGui::BeginPopupModal("New Animation Create"))
			{
				static char strKey[255] = "";
				ImGui::Text("AnimationKey :"); ImGui::SameLine(); ImGui::InputText("##AnimKey", strKey, 255, ImGuiInputTextFlags_EnterReturnsTrue);
				if (CommonUI::ButtonCenteredOnLine("Confirm", 0.3f, 60.f))
				{
					CResMgr::GetInst()->CreateAnimation(StrToWstr(strKey), m_AtlasTex, m_ChangeFrm);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (CommonUI::ButtonCenteredOnLine("Close", 0.f, 60.f))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		if (IsEditMode())
		{
			// 컨펌
			if (CommonUI::ButtonCenteredOnLine("Confirm", 0.5f))
			{
				Ptr<CAnimation2D> pMasterAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(m_pCurAnim->GetKey());
				pMasterAnim->GetFrmVec()->assign(m_ChangeFrm.begin(), m_ChangeFrm.end());
				pMasterAnim->Reallocate();
				ImGui::OpenPopup("Frame Edit");
			}
			if (ImGui::BeginPopupModal("Frame Edit"))
			{
				ImGui::Text("Frame Edit Complete!!!");
				if (CommonUI::ButtonCenteredOnLine("Close", 0.5f))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
}

void AnimTool::FrameImageWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ChildFrameImage", ImVec2(ImGui::GetContentRegionAvail().x, 500.f), true, window_flags);

	int idx = 0;
	if (IsCreateMode())
		idx = m_CreateFrmIdx;
	if (IsEditMode())
		idx = m_EditFrmIdx;

	if (!m_ChangeFrm.empty())
	{
		Vec2 AtlasSize = m_AtlasTex->GetSize();

		Vec2 vFullsize = m_ChangeFrm[idx].vFullSize * AtlasSize;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Vec2 vCursorPos = ImGui::GetCursorScreenPos();
		draw_list->AddRectFilled(vCursorPos, vCursorPos + m_ChangeFrm[idx].vFullSize * AtlasSize, IM_COL32(255, 0, 255, 255));
		draw_list->AddLine(Vec2(vCursorPos.x + vFullsize.x / 2.f, vCursorPos.y),
			Vec2(vCursorPos.x + vFullsize.x / 2.f, vCursorPos.y + vFullsize.y),
			IM_COL32(255, 255, 255, 100));
		draw_list->AddLine(Vec2(vCursorPos.x, vFullsize.y / 2.f + vCursorPos.y),
			Vec2(vCursorPos.x + vFullsize.x, vFullsize.y / 2.f + vCursorPos.y),
			IM_COL32(255, 255, 255, 100));
		// 반투명 전 이미지
		if (idx != 0)
		{
			Vec2 vDiff = (m_ChangeFrm[idx - 1].vFullSize - m_ChangeFrm[idx - 1].vSlice) / 2.f;
			Vec2 vImageStart = vCursorPos + (m_ChangeFrm[idx - 1].vOffset + vDiff) * AtlasSize;
			draw_list->AddImage(m_AtlasSRV,
				vImageStart, vImageStart + m_ChangeFrm[idx - 1].vSlice * AtlasSize,
				m_ChangeFrm[idx - 1].vLeftTop, m_ChangeFrm[idx - 1].vLeftTop + m_ChangeFrm[idx - 1].vSlice, IM_COL32(255, 255, 255, 150));
		}
		Vec2 vDiff = (m_ChangeFrm[idx].vFullSize - m_ChangeFrm[idx].vSlice) / 2.f;
		Vec2 vImageStart = vCursorPos + (m_ChangeFrm[idx].vOffset + vDiff) * AtlasSize;
		draw_list->AddImage(m_AtlasSRV,
			vImageStart, vImageStart + m_ChangeFrm[idx].vSlice * AtlasSize,
			m_ChangeFrm[idx].vLeftTop, m_ChangeFrm[idx].vLeftTop + m_ChangeFrm[idx].vSlice, IM_COL32(255, 255, 255, 255));
	}
	


	ImGui::EndChild();
}

void AnimTool::SetAtlasTex(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = StrToWstr(strKey);

	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	m_AtlasTex = (CTexture*)(mapRes.find(wstrKey)->second.Get());
	m_AtlasSRV = m_AtlasTex->GetSRV().Get();

	m_bImageChanged = true;
}

void AnimTool::DeleteFrame(int& idx)
{
	vector<tAnim2DFrm>::iterator iter = m_ChangeFrm.begin();
	for (; iter != m_ChangeFrm.end(); ++iter)
	{
		if (&(*iter) == &(m_ChangeFrm[idx]))
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

Vec2 AnimTool::GetClickedIdx(Vec2 _vCursorPos)
{
	ImGuiIO& io = ImGui::GetIO();

	// KeyMgr가 받아오는 좌표와 차이가 있음.
	Vec2 vMousePos = io.MousePos;

	Vec2 TexCoord = ((vMousePos - _vCursorPos) / m_vAtlasScale);

	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();

	Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
	Vec2 TexIdx = Vec2((TexCoord.x / vSlice.x), TexCoord.y / vSlice.y);
	TexIdx.x = floor(TexIdx.x);
	TexIdx.y = floor(TexIdx.y);

	return TexIdx;
}

void AnimTool::SetAnimaton(DWORD_PTR _animKey)
{
	string strAnimKey = (char*)_animKey;
	wstring wstrAnimKey = StrToWstr(strAnimKey);

	m_pCurAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(wstrAnimKey).Get();
	m_bAnimChanged = true;
}