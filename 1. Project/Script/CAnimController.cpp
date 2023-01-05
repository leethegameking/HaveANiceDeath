#include "pch.h"
#include "CAnimController.h"

#include "CUnitScript.h"

CAnimController::CAnimController()
	: CScript(int(SCRIPT_TYPE::ANIMCONTROLLER))
	, m_ObjType(OBJ_PLAYER)
	, m_eAnimDir(ANIM_DIR::ANIM_RIGHT)
	, m_bPrevGround(false)
	, m_fSpeed(0.f)
	,m_fPrevSpeed(0.f)
{
}

CAnimController::~CAnimController()
{
}



void CAnimController::begin()
{
	if (GetOwner()->GetScript<CUnitScript>())
	{
		m_ObjType = OBJ_PLAYER;
		m_pAnimNode = mapAnimNode.find(L"animation\\player\\PlayerIdle.anim")->second;
		Animator2D()->Play(m_pAnimNode->pAnimKey);
	}
}

void CAnimController::tick()
{
	GetCondBit();

	m_pPrevAnimNode = m_pAnimNode;
	for (size_t i = 0; i < m_pAnimNode->vecNextAnim.size(); ++i)
	{
		// 아무것도 들어 있지 않은 경우 -> 다음 애니메이션으로 기본 전환. (Run to Idle.)
		if (m_pAnimNode->vecNextAnim[i]->iTranCond == 0)
		{
			if (CalBit(m_pAnimNode->iCond, ANIM_FINISHED, BIT_INCLUDE))
			{
				Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
				m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
				break;
			}
		}
		else
		{
			// Exclude List 빼준 후
			RemoveBit(m_pAnimNode->iCond, m_pAnimNode->vecNextAnim[i]->iExcludeCond);

			if (CalBit(m_pAnimNode->iCond, m_pAnimNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
			{
				Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
				m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
				break;
			}
		}
	}
	
	static bool bOneFrameAfter = false;
	// Uturn 방향전환
	if (CalBit(m_pPrevAnimNode->iPreferences, DIR_CHANGE_ANIM, BIT_INCLUDE))
	{
		if (Animator2D()->FindAnimation(m_pPrevAnimNode->pAnimKey)->IsFinish())
		{
			bOneFrameAfter = true;
		}
	}
	// 공중 방향전환
	else if( !CalBit(m_pAnimNode->iCond, GROUND, BIT_INCLUDE) )
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_eAnimDir = ANIM_DIR::ANIM_LEFT;
			Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		else if (KEY_PRESSED(KEY::D))
		{
			m_eAnimDir = ANIM_DIR::ANIM_RIGHT;
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	// 한 프레임 후에 Animation 방향 뒤집어줌 ( 아예 함수로 render 다음에 넣는 방향도 생각 )
	if (bOneFrameAfter)
	{
		if (m_eAnimDir == ANIM_DIR::ANIM_LEFT)
		{
			m_eAnimDir = ANIM_DIR::ANIM_RIGHT;
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
		else
		{
			m_eAnimDir = ANIM_DIR::ANIM_LEFT;
			Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		bOneFrameAfter = false;
	}
}	

void CAnimController::GetCondBit()
{
	m_pAnimNode->iCond = 0;

	// GROUND
	m_bPrevGround = m_bGround;
	if (Rigidbody2D()->IsGround())
	{
		m_bGround = true;
		AddBit(m_pAnimNode->iCond, GROUND);
	}
	else
	{
		m_bGround = false;
	}

	// ANIM_FINISHED
	if (Animator2D()->GetCurAnim()->OFAFinish())
		AddBit(m_pAnimNode->iCond, ANIM_FINISHED);

	// KEY_A_OR_D
	if( KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D) )
		AddBit(m_pAnimNode->iCond, KEY_A_OR_D);


	// ANIM_DIR_CHANGED
	if (KEY_PRESSED(KEY::A))
	{
		if (m_eAnimDir != ANIM_DIR::ANIM_LEFT)
		{
			if (CalBit(m_pAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
			{
				AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
			}
		}
	}
	else if (KEY_PRESSED(KEY::D))
	{
		if (m_eAnimDir != ANIM_DIR::ANIM_RIGHT)
		{
			if (CalBit(m_pAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
			{
				AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
			}
		}
	}

	// GROUND_TO_AERIAL | AERIAL_TO_GROUND
	if (m_bPrevGround != m_bGround)
	{
 		if(m_bGround == true)
			AddBit(m_pAnimNode->iCond, AERIAL_TO_GROUND);
		else
			AddBit(m_pAnimNode->iCond, GROUND_TO_AERIAL);
	}

	// SPEED_Y_POSITIVE
	m_fPrevSpeed = m_fSpeed;
	m_fSpeed = Rigidbody2D()->GetSpeed().y;
	if (Rigidbody2D()->GetSpeed().y > 0.f)
	{
		AddBit(m_pAnimNode->iCond, SPEED_Y_POSITIVE);
	}

	// SPEED_Y_TURN
	if (m_fPrevSpeed >= 0.f && m_fSpeed < 0.f )
		AddBit(m_pAnimNode->iCond, SPEED_Y_TURN);
	
}

void CAnimController::BeginOverlap(CCollider2D* _other)
{
}

void CAnimController::Overlap(CCollider2D* _other)
{
}

void CAnimController::EndOverlap(CCollider2D* _other)
{
}

void CAnimController::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CAnimController::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}




