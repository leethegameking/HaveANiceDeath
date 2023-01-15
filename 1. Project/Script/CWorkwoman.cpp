#include "pch.h"
#include "CWorkwoman.h"

#include "CAnimController.h"
#include "CEnemyController.h"
#include "CProjectileScript.h"

CWorkwoman::CWorkwoman()
	: CEnemyScript((int)SCRIPT_TYPE::WORKWOMAN)
{
}

CWorkwoman::CWorkwoman(const CWorkwoman& _origin)
	: CEnemyScript(_origin)
{
}

CWorkwoman::~CWorkwoman()
{

}

void CWorkwoman::begin()
{
	CEnemyScript::begin();
	m_eCurPattern = PATTERN_WAITING;
}

void CWorkwoman::tick()
{
	CEnemyScript::tick();

	switch (m_eCurPattern)
	{
	case PATTERN_WAITING:
		WaitState();
		break;
	case PATTERN_APPEAR:
		AppearSatae();
		break;
	case PATTERN_IDLE:
		IdleState();
		break;
	case PATTERN_UTURN:
		UturnState();
		break;
	case PATTERN_DETECT:
		DetectState();
		break;
	case PATTERN_RUN:
		RunState();
		break;
	case PATTERN_ATTACK_READY:
		AttackReadyState();
		break;
	case PATTERN_ATTACK:
		AttackState();
		break;
	case PATTERN_DELAY:
		DelayState();
		break;
	case PATTERN_HIT_START:
		HitStartState();
		break;
	case PATTERN_HIT_LOOP:
		HitLoopState();
		break;
	case PATTERN_HIT_END:
		HitEndState();
		break;
	case PATTERN_DEATH:
		DeathState();
	}
}

void CWorkwoman::BeginOverlap(CCollider2D* _pOther)
{
}

void CWorkwoman::Overlap(CCollider2D* _pOther)
{
	CEnemyScript::Overlap(_pOther);
}

void CWorkwoman::EndOverlap(CCollider2D* _pOther)
{
}

void CWorkwoman::WaitState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkwomanWaiting.anim");
		m_bStateEnter = false;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();

	if (DistanceF(vPlayerPos, vPos) <= m_fAppearRadius)
	{
		m_eCurPattern = PATTERN_APPEAR;
	}
}

void CWorkwoman::AppearSatae()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Appear.anim", false);
		m_bStateEnter = false;
	}

	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		m_eCurPattern = PATTERN_IDLE;
	}
}

void CWorkwoman::IdleState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Idle.anim");
		m_bStateEnter = false;
	}

	// Detect
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();

	if (DistanceF(vPlayerPos, vPos) <= m_fDetectRadius)
	{
		m_eCurPattern = PATTERN_DETECT;
		return;
	}

	// Uturn
	Rigidbody2D()->SetForceSpeedX(m_CurUnitInfo.m_fSpeed * (float)m_CurUnitInfo.m_eDir);

	static float m_fMaxTurnTime = 3.f;
	static float m_fAccTurnTime = 0.f;

	if (m_fMaxTurnTime <= m_fAccTurnTime)
	{
		m_fAccTurnTime = 0.f;
		m_bUturn = true;
		m_eCurPattern = PATTERN_UTURN;
	}
	else
	{
		m_fAccTurnTime += DT;
	}
}

void CWorkwoman::UturnState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Uturn.anim"), false;
		m_bStateEnter = false;
	}

	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		if (m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_RIGHT)
		{
			SetDir(ANIM_DIR::ANIM_LEFT);
			m_bUturn = false;
		}
		else
		{
			SetDir(ANIM_DIR::ANIM_RIGHT);
			m_bUturn = false;
		}
		m_eCurPattern = PATTERN_IDLE;
	}
}

void CWorkwoman::DetectState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Detect.anim", false);
		m_bStateEnter = false;

		// 플레이어 보는 방향으로 설정
		ANIM_DIR eDir = GetAnimDirToPlayer();
		SetDir(eDir);
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_RUN;
	}

}

void CWorkwoman::RunState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Idle.anim");
		m_bStateEnter = false;
	}

	SetDir(GetAnimDirToPlayer());
	Rigidbody2D()->SetForceSpeedX(m_CurUnitInfo.m_fSpeed * (float)m_CurUnitInfo.m_eDir * 2.f);

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();

	if (DistanceF(vPos, vPlayerPos) <= m_fAttackRadius)
	{
		m_eCurPattern = PATTERN_ATTACK_READY;
	}
}

void CWorkwoman::AttackReadyState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Attack_Start.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_ATTACK;
	}
}

void CWorkwoman::AttackState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Attack.anim", false);

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (int)m_CurUnitInfo.m_eDir * 200.f;
		CGameObject* pCloneObj = m_pProjectile->Instantiate();
		Instantiate(pCloneObj, vPos, (int)LAYER_NAME::ENEMY_ATTACK);
		CProjectileScript* pProjScr = pCloneObj->GetScript<CProjectileScript>();
		pProjScr->SetDir(GetDirToPlayer()); // Attack Ready에서 받아오기
		pProjScr->SetAnimationKey(L"animation\\workwoman\\E_Workwoman_Attack_Proj.anim");

		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_DELAY;
	}
}

void CWorkwoman::HitStartState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Hit_Start.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_HIT_LOOP;
	}
}

void CWorkwoman::HitLoopState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Hit_Loop.anim", true);
		m_bStateEnter = false;
	}

	static float fAccTime = 0.f;
	if (m_fFaintTime <= fAccTime)
	{
		m_eCurPattern = PATTERN_HIT_END;
		m_fFaintGauge = 10.f;
		fAccTime = 0.f;
	}
	else
	{
		fAccTime += DT;
	}
}

void CWorkwoman::HitEndState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Hit_End.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_RUN;
	}
}

void CWorkwoman::DelayState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Idle.anim", true);
		m_bStateEnter = false;
	}

	static float fDelayTime = 1.5f;
	static float fAccTime = 0.f;

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();
	if (DistanceF(vPos, vPlayerPos) > m_fAttackRadius)
	{
		SetDir(GetAnimDirToPlayer());
		Rigidbody2D()->SetForceSpeedX(m_CurUnitInfo.m_fSpeed * (float)m_CurUnitInfo.m_eDir * 2.f);
	}

	if (fDelayTime <= fAccTime)
	{
		m_eCurPattern = PATTERN_RUN;
		fAccTime = 0.f;
	}
	else
	{
		fAccTime += DT;
	}
}

void CWorkwoman::DeathState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workwoman\\E_Workwoman_Death.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		Destroy(); // 자신을 지워줌.
	}
}

void CWorkwoman::SaveToFile(FILE* _pFile)
{
	CEnemyScript::SaveToFile(_pFile);
}

void CWorkwoman::LoadFromFile(FILE* _pFile)
{
	CEnemyScript::LoadFromFile(_pFile);
}


