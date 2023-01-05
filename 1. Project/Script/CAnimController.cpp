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
	, m_iCombo(COMBO_NONE)
	, m_iPrevCombo(COMBO_NONE)
	, m_bCombo(false)
	, m_bComboProgress(false)
	, m_pReserveNode(nullptr)
	, m_pTmpSaveNode(nullptr)
	, m_pNextNode(nullptr)
	, m_bDirChanging(false)
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
	m_pPrevAnimNode = m_pAnimNode;
	if (m_pNextNode != nullptr)
	{
		PlayNextNode();
	}
	// GetCombo();
	// 애니메이션 도중 다음 콤보 진행 할 건지
	// SetComboProgress();
	SetCondBit();

	// 콤보 지연
	// ComboDelay();

	// 기본적인 애니메이션 선택 절차
	NodeProgress();

	// 방향 전환 처리
	SetDir();
}	

void CAnimController::SetCondBit()
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
	if (Animator2D()->GetCurAnim()->IsFinish())
		AddBit(m_pAnimNode->iCond, ANIM_FINISHED);

	// KEY_A_OR_D
	if( KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D) )
		AddBit(m_pAnimNode->iCond, KEY_A_OR_D);


	// ANIM_DIR_CHANGED for Uturn
	// DirChanging 중에는 방향을 바꾸지 않음
	if (!m_bDirChanging)
	{
		if (KEY_PRESSED(KEY::A))
		{
			if (m_eAnimDir == ANIM_DIR::ANIM_RIGHT)
			{
				if (CalBit(m_pAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
				{
					AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
				}
			}
		}
		else if (KEY_PRESSED(KEY::D))
		{
			if (m_eAnimDir == ANIM_DIR::ANIM_LEFT)
			{
				if (CalBit(m_pAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
				{
					AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
				}
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
	
	// MOUSE_LEFT
	if (KEY_TAP(KEY::LBTN))
	{
		AddBit(m_pAnimNode->iCond, MOUSE_LEFT);

	}

	// Combo
	//if (m_bBitCombo)
	//	AddBit(m_pAnimNode->iCond, COMBO_PROGRESS);
}



void CAnimController::SetComboProgress()
{
	if (KEY_TAP(KEY::LBTN) && CalBit(m_pAnimNode->iPreferences, COMBO_ANIM, BIT_INCLUDE))
	{
		m_bComboProgress = true;
	}
}

void CAnimController::SetReserveNode()
{
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

void CAnimController::PlayNextNode()
{
	Animator2D()->Play(m_pNextNode->pAnimKey, CalBit(m_pNextNode->iPreferences, REPEAT ,BIT_INCLUDE));
	m_pAnimNode = m_pNextNode;
	m_pNextNode = nullptr;
}

void CAnimController::ComboDelay()
{
	// reserve node 1초동안 기억
	static float fMemorizeTime = 1.f;
	static float fMemorizeAcc = 0.f;

	// 콤보 지연
	if (CalBit(m_pAnimNode->iPreferences, HAS_RESERVE, BIT_INCLUDE))
	{
		m_pReserveNode = m_pAnimNode->pReserveAnim; // 다음 콤보 애니메이션 저장
		m_pTmpSaveNode = m_pAnimNode;
		fMemorizeAcc = 0.f;
	}

	fMemorizeAcc += DT;
	if (fMemorizeAcc > fMemorizeTime || m_bPrevGround != m_bGround)
		m_pReserveNode = nullptr;

	if (m_pReserveNode != nullptr)
	{
		if (!KEY_PRESSED(KEY::W) && !KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN) && m_pTmpSaveNode != m_pAnimNode)
		{
			Animator2D()->Play(m_pReserveNode->pAnimKey);
			m_pAnimNode = m_pReserveNode;
			m_pReserveNode = nullptr;
			SetCondBit();
			m_pPrevAnimNode = m_pAnimNode;
		}
	}
}

void CAnimController::NodeProgress()
{
	for (size_t i = 0; i < m_pAnimNode->vecNextAnim.size(); ++i)
	{
		static UINT iCurCond;
		iCurCond = m_pAnimNode->iCond;

		// Exclude Bit 빼주기
		RemoveBit(iCurCond, m_pAnimNode->vecNextAnim[i]->iExcludeCond);

		if (CalBit(iCurCond, m_pAnimNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
		{
			m_pNextNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
			break;
		}


		// ComboAnimation 전환
		//if (CalBit(iCurCond, ANIM_FINISHED, BIT_INCLUDE) && m_bComboProgress && CalBit(m_pAnimNode->iPreferences, COMBO_ANIM, BIT_INCLUDE))
		//{
		//	Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
		//	m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
		//	m_pReserveNode = nullptr;
		//	m_bComboProgress = false;
		//	break;

		//}
		//// 기본 애니메이션 전환
		//else if (CalBit(iCurCond, m_pAnimNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
		//{
		//	Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
		//	m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
		//	break;
		//}
		//// 아무것도 들어 있지 않은 경우 -> 다음 애니메이션으로 기본 전환. (Run to Idle.)
		//else if (m_pAnimNode->vecNextAnim[i]->iTranCond == 0)
		//{
		//	if (CalBit(iCurCond, ANIM_FINISHED, BIT_INCLUDE))
		//	{
		//		Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
		//		m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
		//		break;
		//	}
		//}
	}
}

void CAnimController::SetDir()
{

	// 한 프레임 나중에 방향 전환 Uturn
	if (m_bDirChanging)
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
		m_bDirChanging = false;
	}

	// Uturn 방향전환
	if (CalBit(m_pAnimNode->iPreferences, DIR_CHANGE_ANIM, BIT_INCLUDE))
	{
		if (Animator2D()->FindAnimation(m_pAnimNode->pAnimKey)->IsFinish())
		{
			m_bDirChanging = true;
		}
	}
	// 공중 or Combo시 방향전환
	else if (!CalBit(m_pAnimNode->iCond, GROUND, BIT_INCLUDE) ||
		(CalBit(m_pAnimNode->iCond, ANIM_FINISHED, BIT_INCLUDE) && CalBit(m_pAnimNode->iPreferences, COMBO_ANIM, BIT_INCLUDE)))
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


}

void tAnimNode::SetReserve(const wstring& _pAnimPath)
{
	pReserveAnim = CAnimController::mapAnimNode.find(_pAnimPath)->second;
}
