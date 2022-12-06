#include "pch.h"
#include "Animator2DUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CAnimator2D.h>


#include "CImGuiMgr.h"
#include "ComboBox.h"
#include "ListUI.h"
#include "AnimCreateTool.h"
#include <Engine/CEventMgr.h>




Animator2DUI::Animator2DUI()
	: ComponentUI("##Animator2DUI", COMPONENT_TYPE::ANIMATOR2D)
	, m_bTargetAnimChanged(false)
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
	int idx = CalComoboPreviewIdx();
	m_AnimComboBox->init(m_vecTargetAnim, idx);

	const map<wstring, Ptr<CRes>>& mapResAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
	map<wstring, Ptr<CRes>>::const_iterator resIter = mapResAnim.begin();

	m_vecResAnim.clear();
	for (; resIter != mapResAnim.end(); ++resIter)
	{
		m_vecResAnim.push_back(WstrToStr(resIter->first));
	}
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

	if (CEventMgr::GetInst()->IsResChanged())
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("ListUI");
		const map<wstring, Ptr<CRes>>& mapResAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
		map<wstring, Ptr<CRes>>::const_iterator resIter = mapResAnim.begin();

		m_vecResAnim.clear();
		for (; resIter != mapResAnim.end(); ++resIter)
		{
			m_vecResAnim.push_back(WstrToStr(resIter->first));
		}
		pListUI->SetItemList(m_vecResAnim);
	}


	if (IsTargetAnimChanged())
	{
		int idx = CalComoboPreviewIdx();
		m_AnimComboBox->init(m_vecTargetAnim, idx);
		m_bTargetAnimChanged = false;
	}
	ImGui::Text("Animation"); ImGui::SameLine(); m_AnimComboBox->render_update();

	// Add Animation Button
	if(ButtonCenteredOnLine("Add Animation", 0.5f))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("ListUI");
		pListUI->AddDynamicConfirm(this, (FUNC_1)&Animator2DUI::AddAnimation);
		pListUI->SetMultiSelect(true);
		// pListUI->SetItemList(vecAnim);
		pListUI->init(m_vecResAnim);
		pListUI->Open();
	}

	// Create Animation
	if (ButtonCenteredOnLine("Create Animation", 0.5f))
	{
		AnimCreateTool* pCreateUI = (AnimCreateTool*)CImGuiMgr::GetInst()->FindUI("AnimCreateTool");
		pCreateUI->SetAutoSize();
		pCreateUI->init();
		pCreateUI->Open();


	}
}

void Animator2DUI::SetCurAnim(DWORD_PTR _animKey)
{
	string strKey = (char*)_animKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	GetTarget()->Animator2D()->Play(wstrKey);
}

void Animator2DUI::AddAnimation(DWORD_PTR _multiSelectIdx)
{
	vector<bool>* multiSelectIdx = (vector<bool>*)_multiSelectIdx;


	// 맵과 벡터를 돌면서 같은 시점이 true면 맵에 있는 애니메이션을 넣어줌.
	const map<wstring, Ptr<CRes>>& mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapAnim.begin();

	int i = 0;
	int idx = 0;
	for (; iter != mapAnim.end(); ++iter)
	{
		if(multiSelectIdx->at(i) == true )
		{
			GetTarget()->Animator2D()->AddAnimation(iter->first);
			m_bTargetAnimChanged = true;
		}
		++i;
	}
}

void Animator2DUI::SetVecTargetAnim()
{
	m_vecTargetAnim.clear();
	const map<wstring, CAnimation2D*> mapAnim = GetTarget()->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*>::const_iterator iter = mapAnim.begin();
	for (; iter != mapAnim.end(); ++iter)
	{
		m_vecTargetAnim.push_back(WstrToStr(iter->first));
	}
}

int Animator2DUI::CalComoboPreviewIdx()
{
	m_vecTargetAnim.clear();
	map<wstring, CAnimation2D*> mapAnim = GetTarget()->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*>::const_iterator iter = mapAnim.begin();
	CAnimation2D* pCurAnim = GetTarget()->Animator2D()->GetCurAnim();

	int i = 0;
	int idx = 0;
	for (; iter != mapAnim.end(); ++iter)
	{
		m_vecTargetAnim.push_back(string(iter->first.begin(), iter->first.end()));
		if (iter->second == pCurAnim)
		{
			idx = i;
		}
		else
		{
			++i;
		}
	}

	return idx;
}

