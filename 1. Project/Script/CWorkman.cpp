#include "pch.h"
#include "CWorkman.h"

#include "CAnimController.h"
#include "CEnemyController.h"
#include "CProjectileScript.h"

CWorkman::CWorkman()
	: CEnemyScript((int)SCRIPT_TYPE::WORKMAN)
	, m_bUturn(false)
{
	m_CurUnitInfo.m_eName = UNIT_NAME::WORKMAN;
}

CWorkman::CWorkman(const CWorkman& _origin)
	: CEnemyScript(_origin)
	, m_bUturn(false)
{
}

CWorkman::~CWorkman()
{

}

void CWorkman::begin()
{
	CEnemyScript::begin();
	m_eCurPattern = PATTERN_WAITING;
}

void CWorkman::tick()
{
	CEnemyScript::tick();

	if (m_eCurPattern != m_ePrevPattern)
	{
		m_bStateEnter = true;
		m_ePrevPattern = m_eCurPattern;
	}

	// Faint가 0이고 HIT 관련 State가 아닐 경우.
	if (m_fFaintGauge < 0.f && !CalBit(m_eCurPattern, PATTERN_HIT_START | PATTERN_HIT_LOOP | PATTERN_HIT_END, BIT_LEAST_ONE))
	{
		m_bStateEnter = true;
		m_ePrevPattern = m_eCurPattern;
		m_eCurPattern = PATTERN_HIT_START;
	}

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
	}
}

void CWorkman::BeginOverlap(CCollider2D* _pOther)
{
}

void CWorkman::Overlap(CCollider2D* _pOther)
{
	CEnemyScript::Overlap(_pOther);
}

void CWorkman::EndOverlap(CCollider2D* _pOther)
{
}

void CWorkman::WaitState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanWaiting.anim");
		m_bStateEnter = false;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();

	if (DistanceF(vPlayerPos, vPos) <= m_fAppearRadius)
	{
		m_eCurPattern = PATTERN_APPEAR;
	}
}

void CWorkman::AppearSatae()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanAppear.anim", false);
		m_bStateEnter = false;
	}

	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		m_eCurPattern = PATTERN_IDLE;
	}
}

void CWorkman::IdleState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanIdle.anim");
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

void CWorkman::UturnState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanUturn.anim"), false;
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

void CWorkman::DetectState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanDetect.anim", false);
		m_bStateEnter = false;

		// 플레이어 보는 방향으로 설정
		ANIM_DIR eDir = GetDirToPlayer();
		SetDir(eDir);
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_RUN;
	}
	
}

void CWorkman::RunState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanRun.anim");
		m_bStateEnter = false;
	}

	SetDir(GetDirToPlayer());
	Rigidbody2D()->SetForceSpeedX(m_CurUnitInfo.m_fSpeed * (float)m_CurUnitInfo.m_eDir * 2.f);

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();

	if (DistanceF(vPos, vPlayerPos) <= m_fAttackRadius)
	{
		m_eCurPattern = PATTERN_ATTACK_READY;
	}
}

void CWorkman::AttackReadyState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanAttackReady.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_ATTACK;
	}
}

void CWorkman::AttackState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanAttack.anim", false);

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (int)m_CurUnitInfo.m_eDir * 200.f;
		CGameObject* pCloneObj = m_pProjectile->Instantiate();
		Instantiate(pCloneObj, vPos, (int)LAYER_NAME::ENEMY_ATTACK);
		CProjectileScript* pProjScr = pCloneObj->GetScript<CProjectileScript>();
		pProjScr->SetDir(Vec2((float)m_CurUnitInfo.m_eDir, 0.f)); // Attack Ready에서 받아오기
		pProjScr->SetAnimOnce();
		pProjScr->SetAnimationKey(L"animation\\workman\\EWorkmanAttackFX.anim");

		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_DELAY;
	}
}

void CWorkman::HitStartState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanHit2_1.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_HIT_LOOP;
	}
}

void CWorkman::HitLoopState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanHit2_2.anim", true);
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

void CWorkman::HitEndState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanHit2_3.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_RUN;
	}
}

void CWorkman::DelayState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanRun.anim", true);
		m_bStateEnter = false;
	}

	static float fDelayTime = 1.5f;
	static float fAccTime = 0.f;

	


	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();
	if (DistanceF(vPos, vPlayerPos) > m_fAttackRadius)
	{
		SetDir(GetDirToPlayer());
		Rigidbody2D()->SetForceSpeedX(m_CurUnitInfo.m_fSpeed * (float)m_CurUnitInfo.m_eDir * 2.f);
	}

	if ( fDelayTime <= fAccTime)
	{
		m_eCurPattern = PATTERN_RUN;
		fAccTime = 0.f;
	}
	else
	{
		fAccTime += DT;
	}
}

void CWorkman::SetDir(ANIM_DIR _eDir)
{
	m_CurUnitInfo.m_eDir = _eDir;
	if(_eDir == ANIM_DIR::ANIM_LEFT)
		Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
	else
		Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
}

bool CWorkman::CurAnimFinish()
{
	if (Animator2D()->GetCurAnim()->IsFinish())
		return true;
	else
		return false;
}

ANIM_DIR CWorkman::GetDirToPlayer()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();
	float fDir = vPlayerPos.x - vPos.x;
	ANIM_DIR eDir;
	if (fDir <= 0.f)
		eDir = ANIM_DIR::ANIM_LEFT;
	else
		eDir = ANIM_DIR::ANIM_RIGHT;

	return eDir;
}

void CWorkman::SaveToFile(FILE* _pFile)
{
	CEnemyScript::SaveToFile(_pFile);
}

void CWorkman::LoadFromFile(FILE* _pFile)
{
	CEnemyScript::LoadFromFile(_pFile);
}


