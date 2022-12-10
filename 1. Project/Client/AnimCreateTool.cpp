#include "pch.h"
#include "AnimCreateTool.h"

#include "imgui_internal.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CResMgr.h>
#include <Engine/CTimeMgr.h>

#include "ComboBox.h"

AnimCreateTool::AnimCreateTool()
	: UI("AnimCreateTool")
	, m_vImageScale(0.5f, 0.5f)
	, m_bUniformed(false)
	, m_vSliceCount(Vec2(10.f, 10.f))
	, m_bHasSelected(false)
	, m_iFrmIdx(0)
	, m_bDragMode(true)
	, m_bImageChanged(true)
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
	m_AtlasComboBox->AddSelectedNotify(this, (FUNC_0)&AnimCreateTool::ClearFrm);
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
	

	// �̹��� ũ�� ǥ��
	ImGui::Text("Width :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_w) + "px"+ " |").c_str());
	ImGui::SameLine();
	ImGui::Text("Height :"); ImGui::SameLine(); ImGui::Text((to_string((int)my_tex_h) + "px").c_str());

	// ������ư 
	if (ImGui::RadioButton("Uniformed Atlas", m_bUniformed == true)) { m_bUniformed = true; m_bHasSelected = false; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Indulgent Atlas", m_bUniformed == false)) { m_bUniformed = false; m_bHasSelected = false; }

	if (IsIndulgent())
	{
		if (ImGui::RadioButton("Drag", m_bDragMode == true)) { m_bDragMode = true; }
		ImGui::SameLine();
		if (ImGui::RadioButton("Auto", m_bDragMode == false)) { m_bDragMode = false; }
	}

	// ���� �Ǿ��� �� slice count ǥ��
	if (IsUniformed())
	{
		static int iSliceCount[2];
		iSliceCount[0] = (int)m_vSliceCount.x;
		iSliceCount[1] = (int)m_vSliceCount.y;
		ImGui::Text("SliceCount"); ImGui::SameLine();
		ImGui::InputInt2("##SliceCount", iSliceCount, ImGuiInputTextFlags_EnterReturnsTrue);
		m_vSliceCount = Vec2(iSliceCount[0], iSliceCount[1]);
	}


	// ���� �������� â �����ؼ� �̹��� ���.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavInputs;
	ImGui::BeginChild("ChildImage", ImVec2(ImGui::GetContentRegionAvail().x, 500.f), true, window_flags);
	Vec2 vCursorPos = ImGui::GetCursorScreenPos();
	ImGui::Image(AtlasSRV, ImVec2(my_tex_w * m_vImageScale.x, my_tex_h * m_vImageScale.y),
		ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.f, 1.f, 1.f, 0.5f));

	// �̹��� Ŭ�� �̺�Ʈ -> Idx��ȯ
	if (IsUniformed())
	{
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
		{
			m_vClickedIdx = GetClickedIdx(vCursorPos);
			m_bHasSelected = true;
		}
		
	}

	// �̹��� Ŭ���� �簢�� �׸��� LT, RB ��ǥ �˾Ƴ�. + Cursor�� �������� 
	if(IsIndulgent())
	{
		ImGuiIO& io = ImGui::GetIO();
		Vec2 vMousePos = io.MousePos;

		static Vec2 vPrevCursor = vCursorPos;

		if (vMousePos.x < vCursorPos.x)
			vMousePos.x = vCursorPos.x;
		if (vMousePos.x > vCursorPos.x + AtlasSize.x * m_vImageScale.x)
			vMousePos.x = vCursorPos.x + AtlasSize.x * m_vImageScale.x;
		if (vMousePos.y < vCursorPos.y)
			vMousePos.y = vCursorPos.y;
		if (vMousePos.y > vCursorPos.y + AtlasSize.y * m_vImageScale.y)
			vMousePos.y = vCursorPos.y + AtlasSize.y * m_vImageScale.y;
		
		Vec2 TexCoord = vMousePos - vCursorPos;

		if (m_bDragMode)
		{
			// ��ũ�� Released������ ȣ��� -> �ӽ� ó��.
			static bool bScrollBool = 0;
			// �׸� �簢�� ��ǥ ���
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
				
				// �簢�� �巡�׷� �����̱�
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && KEY_PRESSED(KEY::LSHIFT))
				{
					Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
					vRectStartPos += Vec2(vMouseDir.x, -vMouseDir.y);
					vRectEndPos += Vec2(vMouseDir.x, -vMouseDir.y);
				}

				// �簢�� Ű�Է����� �����̱�
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
		// �ڵ����ø��
		else
		{
			// �̸� ����
			static vector<vector<tBGRA>> inVec;
			if (m_bImageChanged)
			{
				inVec.clear();
				m_AtlasTex->GetPixelVector(inVec);
				m_bImageChanged = false;
			}

			// �ڵ����� ��ư
			if (ImGui::IsMouseDoubleClicked(0))
			{
				Vec2 AutoSelectPos = Vec2(TexCoord.x / m_vImageScale.x, TexCoord.y / m_vImageScale.y);
				while(true)
				{
					// ����� ���� �ȼ� �߰����� �ٽ� �ʺ�Ž�� ���� 
					Vec4 out = m_AtlasTex->WIdthSearch(inVec, AutoSelectPos);
					AutoSelectPos = Vec2(out.z + 5.f, out.y + (out.w - out.y) / 2.f) ;
					
					// �� �̻� �ȼ��� ã�� ���Ѵٸ� Ż��
					if (out.w - out.y == 0.f)
						break;

					Vec2 LTUV = Vec2(out.x, out.y) /  AtlasSize;
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

			// Ŭ������ ����
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(0))
			{
				if (ImGui::IsMouseClicked(0))
					m_bHasSelected = true;
				
				Vec4 out = m_AtlasTex->WIdthSearch(inVec, TexCoord / m_vImageScale);
				m_AtlasTex->CheckClear(inVec);
				vRectStartPos = Vec2(vCursorPos.x + out.x * m_vImageScale.x, vCursorPos.y + out.y*m_vImageScale.y);
				vRectEndPos = Vec2(vCursorPos.x + out.z * m_vImageScale.x, vCursorPos.y + out.w*m_vImageScale.y);
			}
		}

		Vec2 vChangedCursorPos = vCursorPos - vPrevCursor;

		vRectStartPos += vChangedCursorPos;
		vRectEndPos += vChangedCursorPos;

		vPrevCursor = vCursorPos;

		// ���õ� ���� �׸� �׸���
		if (m_bHasSelected)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRect(vRectStartPos, vRectEndPos, IM_COL32(0, 255, 0, 255));
		}
	}

	// �׸���  �� �簢�� �׸���
	if (IsUniformed())
	{
		// �׸���
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

		// �簢��
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

	// ������ �߰� �� �߰��� ������ �̹�����ư���� ǥ��
	if (IsIndulgent())
	{
		// UV�� ���
		Vec2 LTUV = (vRectStartPos - vCursorPos) / (AtlasSize * m_vImageScale);
		Vec2 RBUV = (vRectEndPos - vCursorPos) / (AtlasSize * m_vImageScale);

		// Frame �߰�
		if (ImGui::Button("Add Frame"))
		{
			// ���콺 �巡�� ��ġ (RT -> LB ����) ��� ����ó��
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
		// ������ �� ��ŭ �̹�����ư���� ǥ��
		
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (size_t i = 0; i < m_ChangeFrm.size(); ++i)
		{
			Vec2 vFrameIdxPos = ImGui::GetCursorScreenPos();
			string imageID = "#imageButton" + to_string(i);
			ImGui::PushID(imageID.c_str());
			if (ImGui::ImageButton(AtlasSRV, Vec2(100.f, 100.f), m_ChangeFrm[i].vLeftTop, m_ChangeFrm[i].vLeftTop + m_ChangeFrm[i].vSlice))
			{
				m_iFrmIdx = i;
			}
			// ����� �ɼ�
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				DeleteFrame(m_iFrmIdx);
				//if (m_iFrmIdx >= i && m_iFrmIdx != 0)
				//	--m_iFrmIdx;

			}
			ImGui::PopID();
			string idx = "[" + to_string(i)  + "]";
			draw_list->AddText(vFrameIdxPos, IM_COL32_WHITE, idx.c_str());

			ImGui::SameLine();
		}
		ImGui::EndChild();
	}

	// ������ ���� �� �ִϸ��̼� ����
	if (IsUniformed())
	{
		static char strKey[255] = "";
		ImGui::Text("AnimationKey"); ImGui::SameLine(); ImGui::InputText("##AnimationKey", strKey, 255);
		ImGui::Text("MaxFrm      "); ImGui::SameLine(); ImGui::InputInt("##MaxFrm", &iMaxFrm, 0, 0);
		ImGui::Text("FPS         "); ImGui::SameLine(); ImGui::InputFloat("##FPS", &fFPS, 0, 0);
		ImGui::Text("FullSize    "); ImGui::SameLine(); ImGui::InputFloat2("##FPS", fFullsize);

		if (ImGui::RadioButton("Horizontal", !IsVertical)) { IsVertical = HORIZONTAL; }
		ImGui::SameLine();
		if(ImGui::RadioButton("Vertical", IsVertical)) { IsVertical = VERTICAL; }
		
		// Animation ����
		Vec2 vSlice = Vec2(my_tex_w / m_vSliceCount.x, my_tex_h / m_vSliceCount.y);
		if (CommonUI::ButtonCenteredOnLine("Create Uniformed Animation", 0.5f))
		{
			
			Vec2 LT = m_vClickedIdx * vSlice;
			float fStep;
			if (IsVertical)
				fStep = vSlice.y;
			else // HORIZONTAL
				fStep = vSlice.x;

			// Animation Create
			CResMgr::GetInst()->CreateAnimation(StrToWstr(strKey), m_AtlasTex, LT, Vec2::Zero, vSlice, fStep, iMaxFrm, fFPS, fFullsize, IsVertical);

			ImGui::OpenPopup("Created");
		}

		// Ȯ�� �˾�.
		if (ImGui::BeginPopupModal("Created"))
		{
			ImGui::Text("Animation Created!!!");
			if (CommonUI::ButtonCenteredOnLine("Close", 0.5f))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

	}

	// ������ ���� �� �ִϸ��̼� ����
	if (IsIndulgent())
	{
		if (!m_ChangeFrm.empty())
		{
			ImGui::PushItemWidth(100.f);
			// ������ ����
			ImGui::Text("LeftTop ");	ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", m_ChangeFrm[m_iFrmIdx].vLeftTop);
			ImGui::Text("Slice   ");	ImGui::SameLine(); ImGui::InputFloat2("##Slice", m_ChangeFrm[m_iFrmIdx].vSlice);
			ImGui::Text("Offset  ");	ImGui::SameLine(); ImGui::InputFloat2("##Offset", m_ChangeFrm[m_iFrmIdx].vOffset);
			ImGui::Text("Fullsize");	ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", m_ChangeFrm[m_iFrmIdx].vFullSize);
			ImGui::Text("Duration");	ImGui::SameLine(); ImGui::InputFloat("##Duation", &m_ChangeFrm[m_iFrmIdx].fDuration, 0, 0);
			ImGui::PopItemWidth();
		}

		// ����
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

	// KeyMgr�� �޾ƿ��� ��ǥ�� ���̰� ����.
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

	m_bImageChanged = true;
}

void AnimCreateTool::DeleteFrame(int& idx)
{
	vector<tAnim2DFrm>::iterator iter = m_ChangeFrm.begin();
	for (; iter != m_ChangeFrm.end(); ++iter)
	{
		if (&(*iter) == &(m_ChangeFrm[m_iFrmIdx]))
		{
			iter = m_ChangeFrm.erase(iter);
			if ( iter != m_ChangeFrm.begin())
			{
				--idx;
			}
			break;
		}
	}
}



