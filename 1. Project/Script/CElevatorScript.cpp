#include "pch.h"
#include "CElevatorScript.h"

#include "CPlayerMgr.h"
#include <Engine/CPrefab.h>
#include <Script/CUIMgr.h>
#include "CLevelSelectScript.h"
#include <Engine/CGameObject.h>

CElevatorScript::CElevatorScript()
	: CScript((int)SCRIPT_TYPE::ELEVATORSCRIPT)
	, m_bSelectUIOn(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Elevator State ", & m_eElevatorState);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "LVSelectPref", &m_pLVSelectPref);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AddTime", &m_fDebugAddTime);
}

CElevatorScript::CElevatorScript(const CElevatorScript& _origin)
	: CScript(_origin)
	, m_bSelectUIOn(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Elevator State ", &m_eElevatorState);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "LVSelectPref", &m_pLVSelectPref);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AddTime", &m_fDebugAddTime);
}

CElevatorScript::~CElevatorScript()
{
}


void CElevatorScript::begin()
{
	m_bFirstState = true;
	m_fDetectRadius = 300.f;
	m_fDebugAddTime = 0.25f;
}

void CElevatorScript::tick()
{  
	static float fDelayTime = 2.f;
	static float fAccTime = 0.f;

	if (fDelayTime <= fAccTime)
	{
		switch (m_eElevatorState)
		{
		case ELEVATOR_APPEAR:
		{
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorAppear.anim", false);
				m_bFirstState = false;
			}

			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				// Player가 서서히 등장.
				m_eElevatorState = ELEVATOR_OPEN;
				m_bFirstState = true;
			}
		}
		break;

		case ELEVATOR_OPEN:
		{
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorOpen.anim", false);
				m_bFirstState = false;
			}

			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				m_eElevatorState = ELEVATOR_WAIT;
				m_bFirstState = true;

				// 엘레베이터 애니메이션 재생
				CPlayerMgr::GetInst()->GetPlayerObj()->Animator2D()->Play(L"animation\\player\\PlayerElevatorIn_Loop.anim");
			}
		}
		break;

		case ELEVATOR_WAIT:
		{
			static float fWaitTime = 2.f;
			static float fWaitAccTime = 0.f;

			if (fWaitTime <= fWaitAccTime)
			{
				m_bFirstState = true;
				CPlayerMgr::GetInst()->SetPlayerDisable(false);
				CPlayerMgr::GetInst()->GetPlayerObj()->Rigidbody2D()->SetIgnGravity(false);
				CPlayerMgr::GetInst()->GetPlayerObj()->Animator2D()->Play(L"animation\\player\\PlayerIdle.anim");

				float fAlpha = 1.f;
				CPlayerMgr::GetInst()->GetPlayerObj()->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fAlpha);
				m_eElevatorState = ELEVATOR_CLOSE;
				fWaitAccTime = 0.f;
			}
			else
			{
				fWaitAccTime += DT;
				float fAlpha = fWaitAccTime / fWaitTime;
				CPlayerMgr::GetInst()->GetPlayerObj()->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fAlpha);
				// player alpha increase
			}
		}
		break;

		case ELEVATOR_CLOSE:
		{
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorClose.anim", false);
				m_bFirstState = false;
			}

			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				// Player가 서서히 등장.
				m_eElevatorState = ELEVATOR_DISAPPEAR;
				m_bFirstState = true;
			}
		}
		break;

		case ELEVATOR_DISAPPEAR:
		{
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorEnd.anim", false);
				m_bFirstState = false;
			}

			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Destroy();
			}
		}
		break;

		case ELEVATOR_WAIT_PLAYER:
		{
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorOpen.anim", false);
				m_pPlayerObj = CPlayerMgr::GetInst()->GetPlayerObj();
				m_bFirstState = false;
			}

			if (m_fDetectRadius > DistanceF(m_pPlayerObj->Transform()->GetWorldPos(), Transform()->GetWorldPos()))
			{
				if (KEY_TAP(KEY::F))
				{
					CPlayerMgr::GetInst()->SetPlayerDisable(true);
					m_pPlayerObj->Rigidbody2D()->SetIgnGravity(true);
					m_eElevatorState = ELEVATOR_IN_RUN;
					m_bFirstState = true;
				}
			}
			// 플레이어와 거리 가까우면 -> F키 누르라는 UI 띄움. 
			// F키 누르면 -> 플레이어 엘레베이터 중앙으로 이동하고 대기 애니메이션 재생
			// F키 누르면 -> 층 고르는 UI 생성
			// UI 상호작용 -> 층 클릭 -> 엘리베이터 Close -> Disappear -> Finish -> 다음 레벨  
		}
		break;

		case ELEVATOR_IN_RUN:
		{
			static float fRunSpeed = 100.f;

			if (m_bFirstState)
			{
				m_pPlayerObj->Animator2D()->Play(L"animation\\player\\PlayerRun.anim", true);
				m_bFirstState = false;
			}

			if (fabs(m_pPlayerObj->Transform()->GetWorldPos().x - Transform()->GetWorldPos().x) < 2.f)
			{
				m_eElevatorState = ELEVATOR_IN;

				// 엘베랑 플레이어 위치 맞추기
				float AddX = 22.f;
				m_pPlayerObj->Transform()->AddRelativePos(Vec3(AddX, 0.f, 0.f));

				m_bFirstState = true;
			}
			else
			{
				float fDir = Transform()->GetWorldPos().x - m_pPlayerObj->Transform()->GetWorldPos().x;
				if (fDir != 0.f)
				{
					fDir = fDir / fabsf(fDir);
					m_pPlayerObj->Transform()->AddRelativePos(Vec3(fDir * fRunSpeed * DT, 0.f, 0.f));
				}
			}
		}
		break;

		case ELEVATOR_IN:
		{
			static float fUpHeight = 15.f;
			static float fUpTime = 4.f;
			static float fUpAccTime = 0.f;
			static float fReduceRatio = 0.3f;
			static Vec3  vScaleOrigin;

			if (m_bFirstState)
			{
				m_pPlayerObj->Animator2D()->Play(L"animation\\player\\PlayerElevatorIn_Loop.anim", true);
				m_bFirstState = false;
				vScaleOrigin = m_pPlayerObj->Transform()->GetRelativeScale();
			}

			if (fUpTime <= fUpAccTime)
			{
				m_eElevatorState = ELEVATOR_SELECT_LV;
				m_bFirstState = true;
				fUpAccTime = 0.f;
			}
			else
			{
				// Height change
				float vHeightChange = fAccTime / fUpTime * DT * fUpHeight;
				m_pPlayerObj->Transform()->AddRelativePos(Vec3(0.f, vHeightChange, 0.f));

				// Scale Change
				float vScaleChange = 1.f - fReduceRatio * fAccTime / fUpTime;
				Vec3 vScaleChanged = vScaleOrigin * vScaleChange;
				m_pPlayerObj->Transform()->SetRelativeScale(vScaleChanged);

				fUpAccTime += DT;
			}
		}
		break;

		case ELEVATOR_SELECT_LV:
		{
			/*static CLevelSelectScript* scrLVSelcet = m_pLVSelectUI->GetChildObject() ->GetScript<CLevelSelectScript>();*/

			if (m_bFirstState)
			{
				m_pLVSelectUI = CUIMgr::GetInst()->GetUIObj(UI_TYPE::UI_SELECT_LV);
				Instantiate(m_pLVSelectUI, Vec3::Zero);
				const  vector<CGameObject*>& vecChildObj = m_pLVSelectUI->GetChildObject();
				for (size_t i = 0; i < vecChildObj.size(); ++i)
				{
					if (vecChildObj[i]->GetName() == L"Level_Select_Boss")
					{
						m_scrLVSelect = vecChildObj[i]->GetScript<CLevelSelectScript>();
						m_scrLVSelect->SetElevator(GetOwner());
						break;
					}
				}
				m_bFirstState = false;
			}

			if (m_scrLVSelect->IsLevelSelected())
			{
				CUIMgr::GetInst()->DestroyUI(UI_TYPE::UI_SELECT_LV);
				m_eElevatorState = ELEVATOR_WAIT_2;
				m_bFirstState = true;
			}
		}
		break;

		case ELEVATOR_WAIT_2:
		{
			static float fWaitTime = 1.f;
			static float fWaitAccTime = 0.f;

			if (fWaitTime <= fWaitAccTime)
			{
				m_eElevatorState = ELEVATOR_CLOSE_2;
				fWaitAccTime = 0.f;
			}
			else
			{
				fWaitAccTime += DT;
			}
		}
		break;

		case ELEVATOR_CLOSE_2:
		{
			static float fSumDuration;
			static float fAccTime;
			static Ptr<CMaterial> pPlayerMtrl;
			
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorClose.anim", false);
				m_bFirstState = false;
				fSumDuration = Animator2D()->GetCurAnim()->GetSumDuration();
				pPlayerMtrl = m_pPlayerObj->MeshRender()->GetCurMaterial();
			}

			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				// Player가 서서히 등장.
				m_eElevatorState = ELEVATOR_DISAPPEAR;
				m_bFirstState = true;
				fAccTime = 0.f;
			}
			else
			{
				float fRatio = (fAccTime + m_fDebugAddTime) / fSumDuration;
				int iElevatorCloseBool = 1;
				
				if (fRatio > 1.f)
					fRatio = 1.f;

				pPlayerMtrl->SetScalarParam(INT_0, &iElevatorCloseBool);
				pPlayerMtrl->SetScalarParam(FLOAT_2, &fRatio);
				fAccTime += DT;
			}
		}
		break;

		}
	}
	else
	{
		fAccTime += DT;
	}
}

void CElevatorScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CElevatorScript::Overlap(CCollider2D* _pOther)
{
}

void CElevatorScript::EndOverlap(CCollider2D* _pOther)
{
}

void CElevatorScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_eElevatorState, sizeof(ELEVATOR_STATE), 1, _pFile);
	SaveResourceRef(m_pLVSelectPref, _pFile);
}

void CElevatorScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_eElevatorState, sizeof(ELEVATOR_STATE), 1, _pFile);
	LoadResourceRef(m_pLVSelectPref, _pFile);
}