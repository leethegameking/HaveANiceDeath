#include "pch.h"
#include "CCutSceneMgr.h"

#include "COrderTextScript.h"

#include <Engine/CGameObject.h>
#include "CCameraMgr.h"
#include "CCamPointWall.h"
#include "CSoundMgr.h"


CCutSceneMgr::CCutSceneMgr()
	: CScript((int)SCRIPT_TYPE::CUTSCENEMGR)
	, m_pEndMarkPref()
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "EndMarkPref", &m_pEndMarkPref);

}

CCutSceneMgr::CCutSceneMgr(const CCutSceneMgr& _origin)
	: CScript(_origin)
	, m_pEndMarkPref(_origin.m_pEndMarkPref)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "EndMarkPref", &m_pEndMarkPref);
}

CCutSceneMgr::~CCutSceneMgr()
{
}

void CCutSceneMgr::begin()
{
	m_scrName = GetOwner()->FindChildObj(L"Text_Name")->GetScript<CBasicTextScript>();
	m_scrContent = GetOwner()->FindChildObj(L"Text_Conversation")->GetScript<CBasicTextScript>();
	m_bCutSceneEnd = false;
	m_iArrIdx = 0;
	m_scrName->SetText(CHAR_CUTSCENE_TEXT_NAME[m_iArrIdx]);
	m_scrContent->SetText(CHAR_CUTSCENE_TEXT_CONTENT[m_iArrIdx]);
	m_bCreateEndMark = true;
	m_bOnce = true;

	CSoundMgr::GetInst()->Play(L"sound\\player\\Player_Dial_1.wav", 1, 0.5f);
}

void CCutSceneMgr::tick()
{
	static CGameObject* pEndMarkClone;
	if (!m_bCutSceneEnd)
	{
		if (dynamic_cast<COrderTextScript*>(m_scrContent)->IsTextOutEnd())
		{
			if (m_bCreateEndMark)
			{
				// EndMark 생성
				pEndMarkClone = m_pEndMarkPref->Instantiate();
				Instantiate(pEndMarkClone, Vec3(46.f, -368.f ,0.f));
				m_bCreateEndMark = false;
			}
			// 텍스트 교체
			if (KEY_TAP(KEY::SPACE))
			{
				if (m_iArrIdx == 0)
				{
					CSoundMgr::GetInst()->Play(L"sound\\brad\\Brad_Dial_1.wav", 1, 0.5f);
				}
				else if (m_iArrIdx == 1)
				{
					CSoundMgr::GetInst()->Play(L"sound\\player\\Player_Dial_2.wav", 1, 0.5f);
				}

				++m_iArrIdx;
				if(m_iArrIdx > 2)
				{
					m_bCutSceneEnd = true;
					return;
				}
				m_scrName->SetText(CHAR_CUTSCENE_TEXT_NAME[m_iArrIdx]);
				m_scrContent->SetText(CHAR_CUTSCENE_TEXT_CONTENT[m_iArrIdx]);
				dynamic_cast<COrderTextScript*>(m_scrContent)->SetTexOutEnd(false);
				
				if (pEndMarkClone)
				{
					pEndMarkClone->Destroy();
					pEndMarkClone = nullptr; 
				}
				m_bCreateEndMark = true;
			}
			
		}
	}
	else
	{
		// 나중에 fade out 셰이더 효과
		static float fFadeTime = 1.f;
		static float fFadeAccTime = 0.f;

		if (pEndMarkClone)
		{
			pEndMarkClone->Destroy();
			pEndMarkClone = nullptr;
		}

		// CamPoint에 Createwall bool -> true
		if (m_bOnce)
		{
			CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM)->GetScript<CCamPointWall>()->CreateWall();
			m_bOnce = false;
		}
		Destroy();
	}
}

void CCutSceneMgr::BeginOverlap(CCollider2D* _pOther)
{
}

void CCutSceneMgr::Overlap(CCollider2D* _pOther)
{
}

void CCutSceneMgr::EndOverlap(CCollider2D* _pOther)
{
}

void CCutSceneMgr::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	SaveResourceRef(m_pEndMarkPref, _pFile);
}

void CCutSceneMgr::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadResourceRef(m_pEndMarkPref, _pFile);
}



