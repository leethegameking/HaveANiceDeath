#include "pch.h"
#include "CElevatorScript.h"

#include "CPlayerMgr.h"
#include <Engine/CPrefab.h>

CElevatorScript::CElevatorScript()
	: CScript((int)SCRIPT_TYPE::ELEVATORSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Elevator State ", & m_eElevatorState);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "LVSelectPref", &m_pLVSelectPref);
}

CElevatorScript::CElevatorScript(const CElevatorScript& _origin)
	: CScript(_origin)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Elevator State ", &m_eElevatorState);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "LVSelectPref", &m_pLVSelectPref);
}

CElevatorScript::~CElevatorScript()
{
}


void CElevatorScript::begin()
{
	m_bFirstState = true;
	m_fDetectRadius = 100.f;
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
			static CGameObject* pPlayerObj;
			if (m_bFirstState)
			{
				Animator2D()->Play(L"animation\\elevator\\ElevatorClose.anim", false);
				m_bFirstState = false;

				pPlayerObj = CPlayerMgr::GetInst()->GetPlayerObj();
			}

			if (m_fDetectRadius > DistanceF(pPlayerObj->Transform()->GetWorldPos(), Transform()->GetWorldPos()))
			{
				Instantiate(m_pLVSelectPref->Instantiate(), Vec3::Zero);
				m_eElevatorState = ELEVATOR_SELECT_LV;
			}
			// 플레이어와 거리 가까우면 -> F키 누르라는 UI 띄움. 
			// F키 누르면 -> 플레이어 엘레베이터 중앙으로 이동하고 대기 애니메이션 재생
			// F키 누르면 -> 층 고르는 UI 생성
			// UI 상호작용 -> 층 클릭 -> 엘리베이터 Close -> Disappear -> Finish -> 다음 레벨  
		}
			break;

		case ELEVATOR_SELECT_LV:
		{

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
	
	bool bPref = false;
	SaveResourceRef(m_pLVSelectPref, _pFile);
}

void CElevatorScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_eElevatorState, sizeof(ELEVATOR_STATE), 1, _pFile);
	bool bPref = false;
	if (m_pLVSelectPref.Get())
		bPref = true;
	fwrite(&bPref, sizeof(bool), 1, _pFile);
	// LoadResourceRef(m_pLVSelectPref, _pFile);
}