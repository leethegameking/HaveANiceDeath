#include "pch.h"
#include "CTitleMenuScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CEventMgr.h>
#include <Engine/CGameObject.h>

#include "CEffectMgr.h"
#include "CCameraMgr.h"
#include "CBasicTextScript.h"
#include "CSoundMgr.h"

CTitleMenuScript::CTitleMenuScript()
	: CScript((int)SCRIPT_TYPE::TITLEMENUSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Menu Type ", &m_eType);
}

CTitleMenuScript::CTitleMenuScript(const CTitleMenuScript& _origin)
	: CScript(_origin)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Menu Type ", &m_eType);
}

CTitleMenuScript::~CTitleMenuScript()
{
}

void CTitleMenuScript::begin()
{
	m_bOnce = true;
}

void CTitleMenuScript::tick()
{
	if (!m_bOnce)
	{
		if (m_pTransition_End->Animator2D()->GetCurAnim().Get())
		{
			if (m_pTransition_End->Animator2D()->GetCurAnim()->IsFinish())
			{
				CEffectMgr::GetInst()->DestroyFX(FX_TYPE::TRANSITION_END);

				if (m_eType == (int)TITLE_MENU_TYPE::PLAY)
				{
					static string strLevel1RelPath = "level\\Level_1.lv";
					tEvent evn;
					evn.eType = EVENT_TYPE::CHANGE_LEVEL;
					evn.wParam = (DWORD_PTR)strLevel1RelPath.data();
					CEventMgr::GetInst()->AddEvent(evn);

					evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
					evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
					CEventMgr::GetInst()->AddEvent(evn);
				}
			}
		}
	}
}

void CTitleMenuScript::BeginOverlap(CCollider2D* _pOther)
{
	float margin = 20.f;
	// Common -> selector pos change
	CGameObject* pSelector = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_NAME::DEFAULT)->FindParentObj(L"Selector");
	Vec2 vColPos = Collider2D()->GetFinalPos();
	Vec2 vColScale = Collider2D()->GetFinalScale();
	pSelector->Transform()->SetRelativePos(vColPos.x - vColScale.x /2.f - margin, vColPos.y, 1.f);

	CSoundMgr::GetInst()->Play(L"sound\\ui\\ui_overlap.wav", 1, 0.8f, true);
}

void CTitleMenuScript::Overlap(CCollider2D* _pOther)
{
	// Play -> Load Level 1 Level State -> Play
	if (KEY_TAP(KEY::LBTN) && m_bOnce)
	{
		m_pTransition_End = CEffectMgr::GetInst()->GetFXObj(FX_TYPE::TRANSITION_END);
		Vec3 vInstantiatePos = CCameraMgr::GetInst()->GetMainCamera()->Transform()->GetWorldPos();
		Instantiate(m_pTransition_End, vInstantiatePos);
		CSoundMgr::GetInst()->Play(L"sound\\ui\\ui_select.wav", 1, 0.8f, true);

		const vector<CGameObject*>& vecChild = GetOwner()->GetParent()->GetChildObject();

		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->GetScript<CBasicTextScript>()->Deactivate();
		}

		m_bOnce = false;
	}
	// Exit -> Exit Program
}

void CTitleMenuScript::EndOverlap(CCollider2D* _pOther)
{
}

void CTitleMenuScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_eType, sizeof(bool), 1, _pFile);
}

void CTitleMenuScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_eType, sizeof(bool), 1, _pFile);
}
