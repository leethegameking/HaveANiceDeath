#include "pch.h"
#include "Animation2DUI.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CResMgr.h>

#include "ComboBox.h"

Animation2DUI::Animation2DUI()
	: UI("##AnimationUI")
{
	m_AtlasComboBox = new ComboBox;

	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);

	map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();

	for (; iter != mapRes.end(); ++iter)
	{
		m_vecRes.push_back(string(iter->first.begin(), iter->first.end()));
	}
}

Animation2DUI::~Animation2DUI()
{
}

void Animation2DUI::init()
{
	// m_AtlasTex = GetTarget()->Animator2D()->GetCurAnim()->GetAtlas();

	for (int i = 0; i < m_vecRes.size(); ++i)
	{
		if (m_AtlasTex.Get()->GetKey() == wstring(m_vecRes[i].begin(), m_vecRes[i].end()))
		{
		//	m_AtlasComboBox->init(, i);
		}
	}
}

void Animation2DUI::update()
{
}

void Animation2DUI::render_update()
{
	ImGui::Text("Image     "); ImGui::SameLine();
	m_AtlasComboBox->render_update();

}
