#include "pch.h"
#include "CBrad.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CRandomMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CUIMgr.h"
#include "CCutSceneMgr.h"
#include "CPlayerMgr.h"
#include "CCameraMgr.h"
#include "CMainCameraScript.h"
#include "CProjectileScript.h"
#include "CCamPointWall.h"
#include "CMainCameraScript.h"

#include "CSoundMgr.h"

CBrad::CBrad()
	: CEnemyScript((int)SCRIPT_TYPE::BRAD)
	, m_fPoseMoveSpeed(500.f)
	, m_fSpinMoveSpeed(500.f)
	, m_fSpinAttackSpeed(500.f)
	, m_fDiveAttackSpeed(800.f)
	, m_fDiveMoveSpeed(500.f)
	, m_fAerialHeight(200.f)
	, m_vToGroundSpeed(100.f, 300.f)
	, m_fDelayTime(1.5f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed - Pose Move", &m_fPoseMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed - Pose Height", &m_fAerialHeight);

	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX Flash", &m_FX_FlashPref);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX Stomp", &m_FX_StompPref);

	AddScriptParam(SCRIPT_PARAM::FLOAT, "PoseMoveSpeed", &m_fPoseMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpinMoveSpeed", &m_fSpinMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpinAttackSpeed", &m_fSpinAttackSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "DiveAttackSpeed", &m_fDiveAttackSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "DiveMoveSpeed", &m_fDiveMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AerialHeight", &m_fAerialHeight);
	AddScriptParam(SCRIPT_PARAM::VEC2, "ToGroundSpeed", &m_vToGroundSpeed);

}

CBrad::CBrad(const CBrad& _origin)
	: CEnemyScript((int)SCRIPT_TYPE::BRAD)
	, m_fPoseMoveSpeed(_origin.m_fPoseMoveSpeed)
	, m_fSpinMoveSpeed(_origin.m_fSpinMoveSpeed)
	, m_fSpinAttackSpeed(_origin.m_fSpinAttackSpeed)
	, m_fDiveMoveSpeed(_origin.m_fDiveMoveSpeed)
	, m_fDiveAttackSpeed(_origin.m_fDiveAttackSpeed)
	, m_fAerialHeight(_origin.m_fAerialHeight)
	, m_vToGroundSpeed(_origin.m_vToGroundSpeed)
	, m_fDelayTime(_origin.m_fDelayTime)
	, m_FX_FlashPref(_origin.m_FX_FlashPref)
	, m_FX_StompPref(_origin.m_FX_StompPref)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed - Pose Move", &m_fPoseMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed - Pose Height", &m_fAerialHeight);	

	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX Flash", &m_FX_FlashPref);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX Stomp", &m_FX_StompPref);

	AddScriptParam(SCRIPT_PARAM::FLOAT, "PoseMoveSpeed", &m_fPoseMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpinMoveSpeed", &m_fSpinMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpinAttackSpeed", &m_fSpinAttackSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "DiveAttackSpeed", &m_fDiveAttackSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "DiveMoveSpeed", &m_fDiveMoveSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "AerialHeight", &m_fAerialHeight);
	AddScriptParam(SCRIPT_PARAM::VEC2, "ToGroundSpeed", &m_vToGroundSpeed);
}

CBrad::~CBrad()
{
}



void CBrad::begin()
{
	CEnemyScript::begin();
	m_eCurPattern = BRAD_INTRO_IDLE;
	SetDir(ANIM_DIR::ANIM_LEFT);
	CPlayerMgr::GetInst()->SetBossObj(GetOwner());

	m_CurUnitInfo.m_fMaxHP = 200.f;
	m_CurUnitInfo.m_fHP = 200.f;
	m_bOnce = true;

	m_fSpinMoveSpeed = 750.f;
	m_fSpinAttackSpeed = 1000.f;
	m_vToGroundSpeed = Vec2(300.f, 500.f);
	m_fDiveAttackSpeed = 1000.f;
	m_fDelayTime = 1.f;
	m_fPoseMoveSpeed = 800.f;
	m_fAerialHeight = 250.f;

	m_ePrevAttackPattern = -1;

}

void CBrad::tick()
{
	CUnitScript::tick();

	if (m_eCurPattern != m_ePrevPattern)
	{
		m_bStateEnter = true;
		m_ePrevPattern = m_eCurPattern;
	}

	if (m_CurUnitInfo.m_fHP <= 0.f)
	{
		Brad_Death();
		return;
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

	ColFromAnimInfo();
	MoveFromAnimInfo();

	m_PrevUnitInfo = m_CurUnitInfo;
}

void CBrad::BeginOverlap(CCollider2D* _pOther)
{
}

void CBrad::Overlap(CCollider2D* _pOther)
{
	CEnemyScript::Overlap(_pOther);
}

void CBrad::EndOverlap(CCollider2D* _pOther)
{
}

void CBrad::SaveToFile(FILE* _pFile)
{
	CEnemyScript::SaveToFile(_pFile);
	SaveResourceRef(m_FX_StompPref, _pFile);
	SaveResourceRef(m_FX_FlashPref, _pFile);
}

void CBrad::LoadFromFile(FILE* _pFile)
{
	CEnemyScript::LoadFromFile(_pFile);
	LoadResourceRef(m_FX_StompPref, _pFile);
	LoadResourceRef(m_FX_FlashPref, _pFile);
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
			Transform()->AddRelativePos(Vec3((float)GetAnimDirToPlayer() * m_fPoseMoveSpeed * DT, 0.f, 0.f));
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
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_SpinAir.anim");
		Rigidbody2D()->SetIgnGravity(true);
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_AirSpin_Loop.wav", 0, 0.5f);

		m_bStateEnter = false;
		vTargetPos = m_pPlayerObj->Transform()->GetRelativePos();
		vTargetPos.y = m_fAerialHeight + CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM)->Transform()->GetWorldPos().y;
		vDir = (vTargetPos - Transform()->GetRelativePos()).Normalize();
	}

	if (Transform()->GetRelativePos().y < vTargetPos.y)
	{
		Vec3 vAdd = vDir * DT * m_fPoseMoveSpeed;
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

		FlashFX();
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Selfie.wav", 1, 0.5f);

		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
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
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_ToIdle.anim", false);
		m_bStateEnter = false;
		StompFX(); // 바닥 충돌 먼지 효과 추가

		tCameraEvent evn;
		evn.eType = CAMERA_EVENT_TYPE::UP_DOWN_SHAKE;
		evn.fMaxTime = 0.3f;

		CCameraMgr::GetInst()->GetMainCamera()->GetScript<CMainCameraScript>()->SetEvent(evn);
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DELAY;
	}
}

void CBrad::Brad_StompReady()
{
	static CGameObject* pCamPoint; 
	static Vec3 vSpinStartPos; 
	static Vec3 vDir;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Ready.anim");
		Rigidbody2D()->SetIgnGravity(true);
		m_bStateEnter = false;

		pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
		vSpinStartPos = pCamPoint->Transform()->GetWorldPos();
		vSpinStartPos.y += m_fAerialHeight;

		vDir = (vSpinStartPos - Transform()->GetRelativePos() ).Normalize();
	}

	if (Transform()->GetRelativePos().y < vSpinStartPos.y)
	{
		MoveToTarget(vSpinStartPos, m_fSpinMoveSpeed, vDir);
	}
	else
	{
		m_eCurPattern = BRAD_STOMP_SPIN_GROUND;
	}

}

void CBrad::Brad_StompSpinGround()
{
	static CGameObject* pCamPoint; 
	static int iMaxCount = 4;
	static int iCurCount = 1;
	static Vec3 fBoundaryR; 
	static Vec3 fBoundaryL; 

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_SpinGround.anim");
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_AirSpin_Loop.wav", 0, 0.5f);
		SetDir(ANIM_DIR::ANIM_RIGHT);
		m_bStateEnter = false;

		pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
		fBoundaryR = pCamPoint->Transform()->GetRelativePos() + Vec3(600.f, 0.f, 0.f);
		fBoundaryL = pCamPoint->Transform()->GetRelativePos() + Vec3(-600.f, 0.f, 0.f);
		iCurCount = 1;
	}

	static float fMaxTime = 0.f;
	static float fAccTime = 0.f;

	if (fMaxTime <= fAccTime)
	{
		CreateSpinRock(); // rock projectile

		float iRandom = (float)GetRandom_D(1, 5);
		iRandom /= 10.f;
		fMaxTime = iRandom;
		fAccTime = 0.f;
	}
	else
	{
		fAccTime += DT;
	}

	if (iMaxCount != iCurCount)
	{
		Transform()->AddRelativePos(Vec3((float)m_CurUnitInfo.m_eDir * m_fSpinAttackSpeed * DT, 0.f, 0.f));

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
		Transform()->AddRelativePos(Vec3((float)m_CurUnitInfo.m_eDir * m_fSpinAttackSpeed * DT, 0.f, 0.f));

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
		if (Transform()->GetWorldPos().x < CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM)->Transform()->GetWorldPos().x)
			SetDir(ANIM_DIR::ANIM_RIGHT);
		else
			SetDir(ANIM_DIR::ANIM_LEFT);

		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Dive_End.wav", 1, 0.5f);

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
		vTargetPos.y = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM)->Transform()->GetWorldPos().y - 1000.f;
		Vec2 vDirToPlayer = GetDirToPlayer();
		vDir = Vec3(vDirToPlayer.x, vDirToPlayer.y, 0.f);
	}

	if (!Rigidbody2D()->IsGround())
	{
		MoveToTarget(vTargetPos, m_vToGroundSpeed);
	}
	else
	{
		Rigidbody2D()->SetIgnGravity(false);
		m_eCurPattern = BRAD_DELAY;
	}
}

void CBrad::Brad_Delay()
{
	static float fDelayAccTime = 0.f;
	static bool bOnce = true;

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Idle.anim");
		m_bStateEnter = false;
	}

	if (m_CurUnitInfo.m_fHP < m_PrevUnitInfo.m_fHP && bOnce)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Hit.anim");
		CSoundMgr::GetInst()->PlayBossMumble(L"sound\\brad\\Brad_Hit_Mumble_3.wav", 1.f, 0.5f);
		bOnce = false;
	}

	if (m_fDelayTime <= fDelayAccTime)
	{
		m_eCurPattern = BRAD_CHASE;
		bOnce = true;
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
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Atk_Combo1.wav", 1, 0.5f);

		m_bStateEnter = false;
	}

	if (CurAnimFinish())
		m_eCurPattern = BRAD_COMBO2_START;
}

void CBrad::Brad_Combo2Start()
{
	if (m_bStateEnter)
	{
		// SetDir(GetAnimDirToPlayer());
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
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Atk_Combo1.wav", 1, 0.5f);

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
}

void CBrad::Brad_Combo3()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Combo3.anim", false);
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Atk_Combo3.wav", 1, 0.5f);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DELAY;
		Rigidbody2D()->SetIgnGravity(false);
	}
}

void CBrad::Brad_DiveMove()
{
	static Vec3 vTargetPos;
	static CGameObject* pCamPoint;
	static Vec3 vCamPointPos;
	static Vec3 vDir;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Idle.anim");
		pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
		vCamPointPos = pCamPoint->Transform()->GetRelativePos();

		m_bStateEnter = false;
		Rigidbody2D()->SetIgnGravity(true);

		if (vCamPointPos.x < Transform()->GetRelativePos().x)
			vTargetPos = vCamPointPos + Vec3(600.f, m_fAerialHeight, 0.f);
		else
			vTargetPos = vCamPointPos + Vec3(-600.f, m_fAerialHeight, 0.f);

		vDir = vTargetPos - Transform()->GetRelativePos();
	}

	if (Transform()->GetRelativePos().y < vTargetPos.y)
		MoveToTarget(vTargetPos, m_fDiveMoveSpeed, vDir);
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
	static float fAddXSum; // x 좌표
	static float fAddXSumPositive = 0.f; // x 변동 값의 합
	static CGameObject* pCamPoint; 
	static Vec3 vCamPointPos; 

	static float fElipseWidth = 610.f;
	static float fElipseHeight = 400.f;

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Dive_Loop.anim");
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Dive_U.wav", 1, 0.5f);
		CSoundMgr::GetInst()->PlayBossMumble(L"sound\\brad\\Brad_Atk_Short_Mumble_3.wav", 1, 0.5f);

		m_bStateEnter = false;

		SelectNextDivePattern();
		pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
		vCamPointPos = pCamPoint->Transform()->GetRelativePos();

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
			float fAddX = m_fDiveAttackSpeed * (float)m_CurUnitInfo.m_eDir * DT;
			vPos.x += fAddX;
			fAddXSum += fAddX;
			fAddXSumPositive += fabsf(fAddX);
			float fElipseY = GetEllipseY(fElipseWidth, fElipseHeight, fAddXSum);
			float fElipseX = Transform()->GetWorldPos().x - vCamPointPos.x;
			vPos.y = fElipseY + vCamPointPos.y + m_fAerialHeight;
			Transform()->SetRelativePos(vPos);

			// rotationa
			float tanTheta = -(fElipseHeight * fElipseHeight * fElipseX) / (fElipseWidth * fElipseWidth * fElipseY);
			float Theta = XM_PI - (atan2f(1.f, tanTheta) + XM_PIDIV2);


			if(m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_RIGHT)
				Transform()->SetRelativeRotation(0.f, 0.f, Theta);
			else
				Transform()->SetRelativeRotation(0.f, XM_PI, Theta);
		}
		else
		{
			m_eCurPattern = BRAD_DIVE_END;
		}
	}
}

void CBrad::Brad_Death()
{
	static Ptr<CAnimation2D> pAnim;
	if (m_bOnce)
	{
		Rigidbody2D()->SetIgnGravity(false);
		SetDir(ANIM_DIR::ANIM_RIGHT);

		Animator2D()->Play(L"animation\\brad\\E_Brad_Death.anim", false);
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Death.wav", 1, 0.5f);

		m_bOnce = false;
		pAnim = Animator2D()->GetCurAnim();

		CPlayerMgr::GetInst()->SetPlayerDisable(true);
		CPlayerMgr::GetInst()->GetPlayerObj()->Animator2D()->Play(L"animation\\player\\PlayerIdle.anim");
		CPlayerMgr::GetInst()->GetPlayerObj()->Rigidbody2D()->SetIgnGravity(false);
		CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM)->GetScript<CCamPointWall>()->DeleteWall();
		CUIMgr::GetInst()->DestroyUI(UI_TYPE::UI_BOSS_HP);
	}
	int iIdx = pAnim->GetCurIdx();

	// 부활 애니메이션 시작
	if (iIdx == 30)
	{
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Outro.wav", 1, 0.5f);
	}

	// 108 -> 뒤돌아가서 가는 애니메이션 시작
	if (iIdx >= 108)
	{
		Vec3 vAdd = Vec3(-1000.f, 0.f, 0.f) * DT;
		Transform()->AddRelativePos(vAdd);

		static float fDestroyTime = 3.f;
		static float fAccTime = 0.f;

		if (fDestroyTime < fAccTime)
		{
			CCameraMgr::GetInst()->GetMainCamera()->GetScript<CMainCameraScript>()->SetMainCamState(MAIN_CAM_STATE::GENERAL);
			Destroy();
			CPlayerMgr::GetInst()->SetPlayerDisable(false);
			fAccTime = 0.f;
		}
		else
			fAccTime += DT;
	}
}

void CBrad::Brad_Hit()
{
}

void CBrad::Brad_IntroIdle()
{
	static float fDelayTime = 0.5f;
	static float fAccTime = 0.f;
	static CMainCameraScript* pScrMainCam;
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Intro_Idle.anim", true);
		pScrMainCam = CCameraMgr::GetInst()->GetMainCamera()->GetScript<CMainCameraScript>();
		m_bStateEnter = false;
	}

	// Trigger ON ( Camera -> Boss )
	if ( pScrMainCam->GetMainCamState() == (int)MAIN_CAM_STATE::BOSS_BRAD )
	{
		if (fDelayTime <= fAccTime)
		{
			m_eCurPattern = BRAD_INTRO_PINGPONG;
			m_bStateEnter = true;
			fAccTime = 0.f;
			// Desk Cre ate Scale(225, 175)
		}
		else
		{
			fAccTime += DT;
		}
	}
}

void CBrad::Brad_IntroPingPong()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Intro_Pingpong.anim", false);
		m_bStateEnter = false;
	}

	// Trigger ON
	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_INTRO_MOVE;
		m_bStateEnter = true;
	}
}

void CBrad::Brad_IntroMove()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Intro_Move.anim", false);
		m_bStateEnter = false;
	}

	// Trigger ON
	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_INTRO_WELCOME;
		m_bStateEnter = true;
		// CutScene Trigger On.
	}
}

void CBrad::Brad_IntroWelcome()
{
	static CGameObject* pUIObj;
	static bool bEntry;
	static bool bOnce;
	static float fDelayTime;
	static float fAccTIme;

	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Intro_Welcome.anim", false);
		m_bStateEnter = false;
		pUIObj = CUIMgr::GetInst()->GetUIObj(UI_TYPE::UI_CUTSCENE);
		bOnce = true;
		bEntry = false;

		fDelayTime = 0.7f;
		fAccTIme = 0.f;
	}



	if (CurAnimFinish() && !bEntry)
	{
		if (fDelayTime <= fAccTIme)
		{
			bEntry = true;
		}
		else
		{
			fAccTIme += DT;
		}
	}


	if (bOnce && bEntry)
	{
		Instantiate(pUIObj, Vec3::Zero);
		bOnce = false;
	}

	if (pUIObj->GetScript<CCutSceneMgr>()->IsCutSceneEnd())
	{
		m_eCurPattern = BRAD_INTRO_TO_FIGHT;
		m_bStateEnter = true;
	}
}


void CBrad::Brad_IntroToFight()
{
	if (m_bStateEnter)
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Intro_To_Fight.anim", false);
		CSoundMgr::GetInst()->PlayBossATK(L"sound\\brad\\Brad_Intro_B.wav", 1, 0.5f);
		m_bStateEnter = false;
	}

	if (CurAnimFinish())
	{
		m_eCurPattern = BRAD_DELAY;
		m_bStateEnter = true;
		CPlayerMgr::GetInst()->SetPlayerDisable(false);
		Instantiate( CUIMgr::GetInst()->GetUIObj(UI_TYPE::UI_BOSS_HP), Vec3::Zero);
	}
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
	m_eNextAttackPattern = GetRandom_D(1, 4);
	if (m_ePrevAttackPattern == m_eNextAttackPattern)
	{
		m_eNextAttackPattern = (m_eNextAttackPattern + 1) % 4 + 1;
	}
	m_ePrevAttackPattern = m_eNextAttackPattern;
}

void CBrad::SelectNextDivePattern()
{
	m_eNextDivePattern = GetRandom_D(2, 2);
}

void CBrad::PlayRandomPose()
{
	int iPoseNum = GetRandom_D(1, 3);
	switch (iPoseNum)
	{
	case 1:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose1.anim", false);
	}
	break;
	case 2:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose2.anim", false);
	}
	break;
	case 3:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose3.anim", false);
	}
	break;
	case 4:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose4.anim", false);
	}
	break;
	case 5:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose5.anim", false);
	}
	break;
	case 6:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose6.anim", false);
	}
	break;
	case 7:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose7.anim", false);
	}
	break;
	case 8:
	{
		Animator2D()->Play(L"animation\\brad\\E_Brad_Stomp_Pose8.anim", false);
	}
	break;
	}
}

void CBrad::FlashFX()
{
	CGameObject* pFXFlashObj = m_FX_FlashPref->Instantiate();
	if (m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_RIGHT)
		pFXFlashObj->Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
	Instantiate(pFXFlashObj, Transform()->GetWorldPos() + Vec3(150.f * (float)m_CurUnitInfo.m_eDir, 0.f, 0.f));
}

void CBrad::StompFX()
{
	CGameObject* pFXStompObj = m_FX_StompPref->Instantiate();
	Instantiate(pFXStompObj, Transform()->GetWorldPos() + Vec3(0.f, -50.f, 0.f));
}

void CBrad::CreateSpinRock()
{
	// rock 
	CGameObject* pRock = m_pProjectile->Instantiate();
	pRock->Rigidbody2D()->SetForceSpeedY(400.f);
	CProjectileScript* scrProj = pRock->GetScript<CProjectileScript>();
	scrProj->SetAnimationKey(L"animation\\brad\\E_Brad_Stomp_Rock.anim");
	Instantiate(pRock, Transform()->GetRelativePos());
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

void CBrad::MoveToTarget(Vec3 _vTarget, Vec2 _vSpeed)
{
	Vec3 vTargetDir = (_vTarget - Transform()->GetWorldPos()).Normalize();

	if (vTargetDir.x > 0.f)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);

	Transform()->AddRelativePos(Vec3(vTargetDir.x * _vSpeed.x * DT, vTargetDir.y * _vSpeed.y * DT, 0.f));
}

void CBrad::MoveToTarget(Vec3 _vTarget, float _fSpeed)
{
	Vec3 vTargetDir = (_vTarget - Transform()->GetWorldPos()).Normalize();

	if (vTargetDir.x > 0.f)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);

	Transform()->AddRelativePos(Vec3(vTargetDir.x * _fSpeed * DT, vTargetDir.y * _fSpeed * DT, 0.f));
}

void CBrad::MoveToTarget(Vec3 _vTarget, float _fSpeed, Vec3 _vDir)
{
	_vDir.Normalize();

	if (_vDir.x > 0.f)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);

	Vec3 vAdd = _vDir * DT * _fSpeed;
	Transform()->AddRelativePos(vAdd);
}

void CBrad::MoveFromAnimInfo()
{
	Vec2 vPosChange = Animator2D()->GetCurAnim()->GetPosChange();
	float vSumDuration = Animator2D()->GetCurAnim()->GetSumDuration();

	Vec2 vCurFramePosChange = vPosChange * DT / vSumDuration;
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += vCurFramePosChange.x * (float)m_CurUnitInfo.m_eDir;
	vPos.y += vCurFramePosChange.y;
	Transform()->SetRelativePos(vPos);
}

void CBrad::ColFromAnimInfo()
{
	Ptr<CAnimation2D> pAnim = Animator2D()->GetCurAnim();
	int idx = pAnim->GetCurIdx();
	const vector<tAnim2DFrm>& vecFrm = pAnim->GetFrmVecRef();

	bool bColOn = vecFrm[idx].bColiiderOn;

	// 오프셋임
	Vec2 vColOffset = vecFrm[idx].iColliderPos;
	Vec2 vColScale = vecFrm[idx].iColliderScale;

	
	if (bColOn)
	{
		m_pAttObj->Collider2D()->SetOffsetPos(Vec2(vColOffset.x * (float)m_CurUnitInfo.m_eDir, vColOffset.y));
		m_pAttObj->Collider2D()->SetScale(vColScale);
	}
	else
	{
		m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * m_pAttObj->GetLayerIdx());
	}
}

void CBrad::ColExpel()
{
	m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * m_pAttObj->GetLayerIdx());
}

void CBrad::ReverseDir()
{
	if (m_CurUnitInfo.m_eDir == ANIM_DIR::ANIM_LEFT)
		SetDir(ANIM_DIR::ANIM_RIGHT);
	else
		SetDir(ANIM_DIR::ANIM_LEFT);
}
