#include "pch.h"
#include "CWorkman.h"

#include "CAnimController.h"
#include "CEnemyController.h"

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
	case PATTERN_ATTACK:
		AttackState();
		break;
	}
}

void CWorkman::BeginOverlap(CCollider2D* _pOther)
{
}

void CWorkman::Overlap(CCollider2D* _pOther)
{
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
		m_eCurPattern = PATTERN_ATTACK;
	}
}

void CWorkman::AttackState()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\workman\\EWorkmanAttack.anim");
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = PATTERN_RUN;
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
	CScript::SaveToFile(_pFile);
}

void CWorkman::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}


