#include "pch.h"
#include "CBrad.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CRandomMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CBrad::CBrad()
	: CEnemyScript((int)SCRIPT_TYPE::BRAD)
{
}

CBrad::CBrad(const CBrad& _origin)
	: CEnemyScript((int)SCRIPT_TYPE::BRAD)
{
}

CBrad::~CBrad()
{
}



void CBrad::begin()
{
	CEnemyScript::begin();
	m_eCurPattern = BRAD_DELAY;
}

void CBrad::tick()
{
	CUnitScript::tick();

	if (m_eCurPattern != m_ePrevPattern)
	{
		m_bStateEnter = true;
		m_ePrevPattern = m_eCurPattern;
	}

	switch (m_eCurPattern)
	{
	case BRAD_COMBO1_START:
		Brad_Combo1Start();
		break;
	case BRAD_COMBO1:
		Brad_Combo1();
		break;
	case BRAD_COMBO2_START:
		Brad_Combo2Start();
		break;
	case BRAD_COMBO2:
		Brad_Combo2();
		break;
	case BRAD_COMBO2_END:
		Brad_Combo2End();
		break;
	case BRAD_COMBO3_START:
		Brad_Combo3Start();
		break;
	case BRAD_COMBO3:
		Brad_Combo3();
		break;
	case BRAD_DEATH:
		Brad_Death();
		break;
	case BRAD_DIVE_READY:
		Brad_DiveReady();
		break;
	case BRAD_DIVE_LOOP:
		Brad_DiveLoop();
		break;
	case BRAD_DIVE_END:
		Brad_DiveEnd();
		break;
	case BRAD_HIT:
		Brad_Hit();
		break;
	case BRAD_CHASE:
		Brad_Chase();
		break;
	case BRAD_INTRO_IDLE:
		Brad_IntroIdle();
		break;
	case BRAD_INTRO_DESK:
		Brad_IntroDesk();
		break;
	case BRAD_INTRO_MOVE:
		Brad_IntroMove();
		break;
	case BRAD_INTRO_PINGPONG:
		Brad_IntroPingPong();
		break;
	case BRAD_INTRO_TO_FIGHT:
		Brad_IntroToFight();
		break;
	case BRAD_INTRO_WELCOME:
		Brad_IntroWelcome();
		break;
	case BRAD_STOMP_POSE:
		Brad_StompPose();
		break;
	case BRAD_STOMP_READY:
		Brad_StompReady();
		break;
	case BRAD_STOMP_ROCK:
		Brad_StompRock();
		break;
	case BRAD_STOMP_SPIN_AIR:
		Brad_StompSpinAir();
		break;
	case BRAD_STOMP_SPIN_GROUND:
		Brad_StompSpinGround();
		break;
	case BRAD_STOMP_TO_IDLE:
		Brad_StompToIdle();
		break;
	case BRAD_STUN_LOOP:
		Brad_StunLoop();
		break;
	case BRAD_STUN_START:
		Brad_StunStart();
		break;
	case BRAD_STUN_END:
		Brad_StunEnd();
		break;
	case BRAD_UTURN:
		Brad_Uturn();
		break;
	case BRAD_DELAY:
		Brad_Delay();
		break;
	case BRAD_TO_GROUND:
		Brad_ToGround();
		break;
	case BRAD_DIVE_MOVE:
		Brad_DiveMove();
		break;
	}
}

void CBrad::BeginOverlap(CCollider2D* _pOther)
{
}

void CBrad::Overlap(CCollider2D* _pOther)
{
}

void CBrad::EndOverlap(CCollider2D* _pOther)
{
}

void CBrad::SaveToFile(FILE* _pFile)
{
	CEnemyScript::SaveToFile(_pFile);
}

void CBrad::LoadFromFile(FILE* _pFile)
{
	CEnemyScript::LoadFromFile(_pFile);
}

// ==================================== PATTERN ======================================

void CBrad::Brad_Chase()
{
	if (m_bStateEnter)
	{
		m_bStateEnter = false;
		SelectNextAttackPattern();
	}

	switch (m_eNextAttackPattern)
	{
	case POSE:
	{
		m_eCurPattern = BRAD_STOMP_SPIN_AIR;
	}
	break;

	case SPIN_ROCK:
	{
		m_eCurPattern = BRAD_STOMP_READY;
	}
	break;

	case COMBO:
	{
		if (DistanceF(m_pPlayerObj->Transform()->GetRelativePos(), Transform()->GetRelativePos()) < m_fAttackRadius)
		{
			m_eCurPattern = BRAD_COMBO1_START;
		}
		else
		{
			SetDir(GetAnimDirToPlayer());
			Transform()->AddRelativePos(Vec3((float)GetAnimDirToPlayer() * m_CurUnitInfo.m_fSpeed * DT, 0.f, 0.f));
		}
	}
	break;

	case DIVE:
	{
		m_eCurPattern = BRAD_DIVE_MOVE;
	}
	break;
	}
}

void CBrad::Brad_StompSpinAir()
{
	static Vec3 vTargetPos;
	static Vec3 vDir;
	static float fMoveSpeed = 100.f;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_SpinAir.anim");
		Rigidbody2D()->SetIgnGravity(true);
		m_bStateEnter = false;
		vTargetPos = m_pPlayerObj->Transform()->GetRelativePos();
		vTargetPos.y += 200.f;
		vDir = (vTargetPos - Transform()->GetRelativePos()).Normalize();
	}

	if (Transform()->GetRelativePos().y < vTargetPos.y)
	{
		Vec3 vAdd = vDir * DT * fMoveSpeed;
		Transform()->AddRelativePos(vAdd);
	}

	else
	{
		m_eCurPattern = BRAD_STOMP_POSE;
	}	
}


void CBrad::Brad_StompPose()
{
	if (m_bStateEnter)
	{
		PlayRandomPose();
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		FlashFX(); // bool 값 추가후 끝나면 중력 없애기.
		Rigidbody2D()->SetIgnGravity(false);
	}

	if (Rigidbody2D()->IsGround())
	{
		m_eCurPattern = BRAD_STOMP_TO_IDLE;
	}
}

void CBrad::Brad_StompToIdle()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_ToIdle.anim");
		m_bStateEnter = false;
		StompFX(); // 바닥 충돌 먼지 효과 추가
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DELAY;
	}
}

void CBrad::Brad_StompReady()
{
	static CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_NAME::DEFAULT)->FindParentObj(L"MainCamera");
	static Vec3 vSpinStartPos = pMainCam->Transform()->GetRelativePos() + Vec3(0.f, 100.f, 0.f);
	static Vec3 vDir;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Ready.anim");
		Rigidbody2D()->SetIgnGravity(true);
		m_bStateEnter = false;
		vDir = (vSpinStartPos - Transform()->GetRelativePos() ).Normalize();
	}

	if (Transform()->GetRelativePos().y < vSpinStartPos.y)
	{
		MoveToTarget(vSpinStartPos, Vec2(0.f , 100.f), vDir);
	}
	else
	{
		m_eCurPattern = BRAD_STOMP_SPIN_GROUND;
	}

}

void CBrad::Brad_StompSpinGround()
{
	static CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_NAME::DEFAULT)->FindParentObj(L"MainCamera");
	static int iMaxCount = 4;
	static int iCurCount = 1;
	static float fMoveSpeed = 200.f;
	static Vec3 fBoundaryR = pMainCam->Transform()->GetRelativePos() + Vec3(600.f, 0.f, 0.f);
	static Vec3 fBoundaryL = pMainCam->Transform()->GetRelativePos() + Vec3(-600.f, 0.f, 0.f);

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_SpinGround.anim");
		SetDir(ANIM_DIR::ANIM_RIGHT);
		m_bStateEnter = false;
		iCurCount = 1;
	}

	CreateSpinRock(); // rock projectile

	if (iMaxCount != iCurCount)
	{
		Transform()->AddRelativePos(Vec3((float)m_CurUnitInfo.m_eDir * fMoveSpeed * DT, 0.f, 0.f));

		if (fBoundaryL.x >= Transform()->GetRelativePos().x && m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_LEFT)
		{
			m_CurUnitInfo.m_eDir = ANIM_DIR::ANIM_RIGHT;
			++iCurCount;
		}
		else if (fBoundaryR.x <= Transform()->GetRelativePos().x && m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_RIGHT)
		{
			m_CurUnitInfo.m_eDir = ANIM_DIR::ANIM_LEFT;
			++iCurCount;
		}
	}
	else
	{
		Transform()->AddRelativePos(Vec3((float)m_CurUnitInfo.m_eDir * fMoveSpeed * DT, 0.f, 0.f));

		// spin rock attack last pos
		if (fBoundaryR.x - 150.f <= Transform()->GetRelativePos().x)
		{
			m_eCurPattern = BRAD_DIVE_END;
		}
	}
}

void CBrad::Brad_DiveEnd()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Dive_End.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_TO_GROUND;
}

void CBrad::Brad_ToGround()
{
	static Vec3 vTargetPos;
	static Vec3 vDir;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Idle.anim");
		m_bStateEnter = false;
		SetDir(GetAnimDirToPlayer());
		vTargetPos = m_pPlayerObj->Transform()->GetRelativePos();
		Vec2 vDirToPlayer = GetDirToPlayer();
		vDir = Vec3(vDirToPlayer.x, vDirToPlayer.y, 0.f);
	}

	if (!Rigidbody2D()->IsGround())
	{
		MoveToTarget(vTargetPos, Vec2(100.f, 300.f), vDir);
	}
	else
	{
		Rigidbody2D()->SetIgnGravity(false);
		m_eCurPattern = BRAD_DELAY;
	}
}

void CBrad::Brad_Delay()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Idle.anim");
		m_bStateEnter = false;
	}

	static float fDelayTime = 2.f;
	static float fDelayAccTime = 0.f;

	if (fDelayTime <= fDelayAccTime)
	{
		m_eCurPattern = BRAD_CHASE;
		fDelayAccTime = 0.f;
	}
	else
	{
		fDelayAccTime += DT;
	}
}

void CBrad::Brad_Combo1Start()
{
	if (m_bStateEnter)
	{
		SetDir(GetAnimDirToPlayer());
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo1_Start.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_COMBO1;
}

void CBrad::Brad_Combo1()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo1.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_COMBO2_START;
	else
		MoveFromAnimInfo();
}

void CBrad::Brad_Combo2Start()
{
	if (m_bStateEnter)
	{
		SetDir(GetAnimDirToPlayer());
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo2_Start.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_COMBO2;
}

void CBrad::Brad_Combo2()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo2.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		if (DistanceF(Transform()->GetRelativePos(), m_pPlayerObj->Transform()->GetRelativePos()) < m_fAttackRadius)
		{
			m_eCurPattern = BRAD_COMBO3_START;
		}
		else
		{
			m_eCurPattern = BRAD_COMBO2_END;
		}
	}
	else
		MoveFromAnimInfo();
}

void CBrad::Brad_Combo2End()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo2_End.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_DELAY;
}

void CBrad::Brad_Combo3Start()
{
	if (m_bStateEnter)
	{
		SetDir(GetAnimDirToPlayer());
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo3_Start.anim", false);
		Rigidbody2D()->SetIgnGravity(true);
		m_bStateEnter = false;
	}

	if(CurAnimFinish())
		m_eCurPattern = BRAD_COMBO3;
	else
	{
		MoveFromAnimInfo();
	}
}

void CBrad::Brad_Combo3()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo3.anim", false);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DELAY;
		Rigidbody2D()->SetIgnGravity(false);
	}
	else
	{
		MoveFromAnimInfo();
	}
}

void CBrad::Brad_DiveMove()
{
	static Vec3 vTargetPos;
	static CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_NAME::DEFAULT)->FindParentObj(L"MainCamera");
	static Vec3 vCamPos = pMainCam->Transform()->GetRelativePos();
	static Vec3 vDir;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Idle.anim");
		m_bStateEnter = false;
		Rigidbody2D()->SetIgnGravity(true);

		if (vCamPos.x < Transform()->GetRelativePos().x)
			vTargetPos = vCamPos + Vec3(600.f, 200.f, 0.f);
		else
			vTargetPos = vCamPos + Vec3(-600.f, 200.f, 0.f);

		vDir = vTargetPos - Transform()->GetRelativePos();
	}

	if (Transform()->GetRelativePos().y < vTargetPos.y)
		MoveToTarget(vTargetPos, Vec2(300.f, 300.f), vDir);
	else
 		m_eCurPattern = BRAD_DIVE_READY;
}


void CBrad::Brad_DiveReady()
{
	static CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_NAME::DEFAULT)->FindParentObj(L"MainCamera");
	static Vec3 vCamPos = pMainCam->Transform()->GetRelativePos();

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Dive_Ready.anim", false);

		if (vCamPos.x < Transform()->GetRelativePos().x)
			SetDir(ANIM_DIR::ANIM_LEFT);
		else
			SetDir(ANIM_DIR::ANIM_RIGHT);

		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DIVE_LOOP;
	}
}

void CBrad::Brad_DiveLoop()
{
	static float fSpeed = 600.f;
	static float fAddXSum;
	static float fAddXSumPositive = 0.f;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Dive_Loop.anim");
		m_bStateEnter = false;
		SelectNextDivePattern();

		if (m_eNextDivePattern == DIVE_U)
		{
			fAddXSum = (float)m_CurUnitInfo.m_eDir * -600.f;
			fAddXSumPositive = 0.f;
		}
	}

	if (m_eNextDivePattern == DIVE_CROSS)
	{
		MoveFromAnimInfo();

		if (CurAnimFinish())
		{
			Rigidbody2D()->SetIgnGravity(false);
			m_eCurPattern = BRAD_DELAY;
		}
	}
	else if (m_eNextDivePattern == DIVE_U)
	{		
		if (fAddXSumPositive <= 1200.f)
		{
			Vec3 vPos = Transform()->GetRelativePos();
			float fAddX = fSpeed * (float)m_CurUnitInfo.m_eDir * DT;
			vPos.x += fAddX;
			fAddXSum += fAddX;
			fAddXSumPositive += fabsf(fAddX);
			vPos.y = GetEllipseY(610.f, 500.f, fAddXSum) + 300.f;
			Transform()->SetRelativePos(vPos);
		}
		else
		{
			m_eCurPattern = BRAD_DIVE_END;
		}
	}
}

void CBrad::Brad_Death()
{
}






void CBrad::Brad_Hit()
{
}

void CBrad::Brad_IntroIdle()
{
}

void CBrad::Brad_IntroDesk()
{
}

void CBrad::Brad_IntroMove()
{
}

void CBrad::Brad_IntroPingPong()
{
}

void CBrad::Brad_IntroToFight()
{
}

void CBrad::Brad_IntroWelcome()
{
}

void CBrad::Brad_StompRock()
{
}

void CBrad::Brad_StunStart()
{
}

void CBrad::Brad_StunLoop()
{
}

void CBrad::Brad_StunEnd()
{
}

void CBrad::Brad_Uturn()
{
}




//====================================================================== Pattern End ===============================================================================

void CBrad::SelectNextAttackPattern()
{
	m_eNextAttackPattern = GetRandom_D(4, 4);
}

void CBrad::SelectNextDivePattern()
{
	m_eNextDivePattern = GetRandom_D(2, 2);
}

void CBrad::PlayRandomPose()
{
	int iPoseNum = GetRandom_D(1, 8);
	switch (iPoseNum)
	{
	case 1:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose1.anim");
	}
	break;
	case 2:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose2.anim");
	}
	break;
	case 3:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose3.anim");
	}
	break;
	case 4:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose4.anim");
	}
	break;
	case 5:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose5.anim");
	}
	break;
	case 6:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose6.anim");
	}
	break;
	case 7:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose7.anim");
	}
	break;
	case 8:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose8.anim");
	}
	break;
	}
}

void CBrad::FlashFX()
{
}

void CBrad::StompFX()
{
}

void CBrad::CreateSpinRock()
{
}

void CBrad::MoveToTarget(Vec3 _vTarget, Vec2 _vSpeed, Vec3 _vDir)
{
	_vDir.Normalize();

	if (_vDir.x > 0.f)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);

	Vec3 vAdd = Vec3(_vDir.x * _vSpeed.x, _vDir.y * _vSpeed.y, 0.f) * DT;
	Transform()->AddRelativePos(vAdd);
}

void CBrad::MoveFromAnimInfo()
{
	Vec2 vPosChamge = Animator2D()->GetCurAnim()->GetPosChange();
	Transform()->AddRelativePos(Vec3((float)m_CurUnitInfo.m_eDir * vPosChamge.x, vPosChamge.y, 0.f));
}

void CBrad::ReverseDir()
{
	if (m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_LEFT)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);
}
