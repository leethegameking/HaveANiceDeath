#include "pch.h"
#include "Animation2DUI.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CResMgr.h>

#include "ComboBox.h"

Animation2DUI::Animation2DUI()
	: UI("AnimationUI")
	, m_vImageScale(0.5f, 0.5f)
{
	m_AtlasComboBox = new ComboBox;
	Close();
}

Animation2DUI::~Animation2DUI()
{
	if (m_AtlasComboBox)
		delete m_AtlasComboBox;
}



void Animation2DUI::init()
{
	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
	m_AtlasTex = (CTexture*)iter->second.Get();

	m_vecRes.clear();
	for (; iter != mapRes.end(); ++iter)
	{
		m_vecRes.push_back(string(iter->first.begin(), iter->first.end()));
	}
	m_AtlasComboBox->AddSelectedFunc(this, (FUNC_1)&Animation2DUI::SetAtlasTex);
	m_AtlasComboBox->init(m_vecRes, 0);
}

void Animation2DUI::update()
{
}

void Animation2DUI::render_update()
{
	ImGui::Text("Image     "); ImGui::SameLine();
	m_AtlasComboBox->render_update();

	// UI ImageScale
	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("##ImageScaleX", &m_vImageScale.x, 0.1f);
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vImageScale.y, 0.1f);
	ImGui::PopItemWidth();

	if (m_AtlasTex.Get())
	{
		ImTextureID AtlasSRV = m_AtlasTex->GetSRV().Get();
		float my_tex_w = m_AtlasTex->GetWidth();
		float my_tex_h = m_AtlasTex->GetHeight();

		ImGui::Image(AtlasSRV, ImVec2(my_tex_w * m_vImageScale.x, my_tex_h * m_vImageScale.y));
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
		{

		}
	}
}

void Animation2DUI::Close()
{
	m_vecRes.clear();
	UI::Close();
}

void Animation2DUI::SetAtlasTex(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = StrToWstr(strKey);

	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
	m_AtlasTex = (CTexture*)(mapRes.find(wstrKey)->second.Get());
}

