#include "pch.h"
#include "Animator2DUI.h"

#include "ComboBox.h"
#include <Engine/CAnimator2D.h>

Animator2DUI::Animator2DUI()
	: ComponentUI("##Animator2DUI", COMPONENT_TYPE::ANIMATOR2D)
{
	m_AnimComboBox = new ComboBox;
	m_AnimComboBox->AddSelectedFunc(this, (FUNC_1)&Animator2DUI::SetCurAnim);
}

Animator2DUI::~Animator2DUI()
{
	if (m_AnimComboBox)
		delete m_AnimComboBox;
}



void Animator2DUI::init()
{
	map<wstring, CAnimation2D*> mapAnim = GetTarget()->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*>::const_iterator iter = mapAnim.begin();
	CAnimation2D* pCurAnim = GetTarget()->Animator2D()->GetCurAnim();

	int i = 0;
	int idx = 0;
	for (; iter != mapAnim.end(); ++iter)
	{
		m_vecAnim.push_back(string(iter->first.begin(), iter->first.end()));
		if (iter->second == pCurAnim)
		{
			idx = i;
		}
		else
		{
			++i;
		}
	}
	
	m_AnimComboBox->init(m_vecAnim, idx);
}

void Animator2DUI::update()
{
	if (GetTarget())
	{
		m_pCurAnim = GetTarget()->Animator2D()->GetCurAnim();
		m_bRepeat = GetTarget()->Animator2D()->IsRepeat();
	}

	ComponentUI::update();
}

void Animator2DUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("Animation"); ImGui::SameLine(); m_AnimComboBox->render_update();
	

}

void Animator2DUI::SetCurAnim(DWORD_PTR _animKey)
{
	string strKey = (char*)_animKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	
	GetTarget()->Animator2D()->Play(wstrKey);
}

