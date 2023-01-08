#include "pch.h"
#include "CAnimController.h"

#include "CUnitScript.h"

CAnimController::CAnimController()
	: CScript(int(SCRIPT_TYPE::ANIMCONTROLLER))
	, m_eCurAnimDir(ANIM_DIR::ANIM_RIGHT)
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
	, m_eNextDir(ANIM_DIR::END)
{
}

CAnimController::CAnimController(int _ScriptType)
	: CScript(_ScriptType)
	, m_eCurAnimDir(ANIM_DIR::ANIM_RIGHT)
	, m_bPrevGround(false)
	, m_fSpeed(0.f)
	, m_fPrevSpeed(0.f)
	, m_iCombo(COMBO_NONE)
	, m_iPrevCombo(COMBO_NONE)
	, m_bCombo(false)
	, m_bComboProgress(false)
	, m_pReserveNode(nullptr)
	, m_pTmpSaveNode(nullptr)
	, m_pNextNode(nullptr)
	, m_bDirChanging(false)
	, m_eNextDir(ANIM_DIR::END)
	, m_bCanDash(true)
{
}

CAnimController::~CAnimController()
{
}

void CAnimController::begin()
{
	const vector<CGameObject*>& vecChildObj = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChildObj.size(); ++i)
	{
		if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::PLAYER_HIT)
		{
			m_pHitObj = vecChildObj[i];
		}
		else if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::PLAYER_ATTACK)
		{
			m_pAttObj = vecChildObj[i];
		}
	}

	m_pUnitScr = GetOwner()->GetScript<CUnitScript>();
}

void CAnimController::tick()
{
	m_pPrevAnimNode = m_pCurAnimNode;
	if (m_pNextNode != nullptr)
	{
		PlayNextNode();
	}
	Timer();
	SetCurDir();
	SetCondBit();
	SetGravity();
	SetAttackCollider();
	SetInvincible();
	PosChangeProgress();
	
	// 기본적인 애니메이션 선택 절차 + 콤보 지연
	NodeProgress();

	// 방향 전환 처리
	SetDir();
}	

void CAnimController::SetCondBit()
{
	m_pCurAnimNode->iCond = 0;

	// GROUND
	m_bPrevGround = m_bGround;
	if (Rigidbody2D()->IsGround())
	{
		m_bGround = true;
		AddBit(m_pCurAnimNode->iCond, GROUND);
	}
	else
	{
		m_bGround = false;
	}

	// ANIM_FINISHED
	if (Animator2D()->GetCurAnim()->IsFinish())
		AddBit(m_pCurAnimNode->iCond, ANIM_FINISHED);

	// KEY_A_OR_D
	if( KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D) )
		AddBit(m_pCurAnimNode->iCond, KEY_A_OR_D);


	// ANIM_DIR_CHANGED for Uturn
	// DirChanging 중에는 방향을 바꾸지 않음
	if (!m_bDirChanging)
	{
		if (KEY_PRESSED(KEY::A))
		{
			if (m_eCurAnimDir == ANIM_DIR::ANIM_RIGHT)
			{
				if (CalBit(m_pCurAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
				{
					AddBit(m_pCurAnimNode->iCond, ANIM_DIR_CHANGED);
				}
			}
		}
		else if (KEY_PRESSED(KEY::D))
		{
			if (m_eCurAnimDir == ANIM_DIR::ANIM_LEFT)
			{
				if (CalBit(m_pCurAnimNode->iPreferences, NEED_DIR_CHANGE, BIT_INCLUDE))
				{
					AddBit(m_pCurAnimNode->iCond, ANIM_DIR_CHANGED);
				}
			}
		}
	}

	// GROUND_TO_AERIAL | AERIAL_TO_GROUND
	if (m_bPrevGround != m_bGround)
	{
 		if(m_bGround == true)
			AddBit(m_pCurAnimNode->iCond, AERIAL_TO_GROUND);
		else
			AddBit(m_pCurAnimNode->iCond, GROUND_TO_AERIAL);
	}

	// SPEED_Y_POSITIVE
	m_fPrevSpeed = m_fSpeed;
	m_fSpeed = Rigidbody2D()->GetSpeed().y;
	if (Rigidbody2D()->GetSpeed().y > 0.f)
	{
		AddBit(m_pCurAnimNode->iCond, SPEED_Y_POSITIVE);
	}
	// SPEED_Y_NEGATIVE
	else if (Rigidbody2D()->GetSpeed().y < 0.f)
	{
		AddBit(m_pCurAnimNode->iCond, SPEED_Y_NEGATIVE);
	}

	//// SPEED_Y_TURN
	//if (m_fPrevSpeed >= 0.f && m_fSpeed < 0.f)
	//	AddBit(m_pCurAnimNode->iCond, SPEED_Y_TURN);
	
	// MOUSE_LEFT
	if (KEY_TAP(KEY::LBTN))
	{
		AddBit(m_pCurAnimNode->iCond, MOUSE_LEFT);
	}

	SetComboProgress();

	// Combo
	if (m_bComboProgress)
		AddBit(m_pCurAnimNode->iCond, COMBO_PROGRESS);

	if(m_bCanDash)
		AddBit(m_pCurAnimNode->iCond, CAN_DASH);

	// KEY_SHIFT
	if (KEY_TAP(KEY::LSHIFT))
	{
		AddBit(m_pCurAnimNode->iCond, KEY_SHIFT);
		if (m_bCanDash)
		{
			m_bCanDash = false;
		}
	}

	// HP_DOWN
	if (CalBit(m_pUnitScr->GetUnitState(), UNIT_HP_DOWN, BIT_INCLUDE))
	{
		m_pUnitScr->RemoveUnitState(UNIT_HP_DOWN);
		AddBit(m_pCurAnimNode->iCond, HP_DOWN);
	}

	// HPZERO
	if (CalBit(m_pUnitScr->GetUnitState(), UNIT_HP_ZERO, BIT_INCLUDE))
	{
		m_pUnitScr->RemoveUnitState(UNIT_HP_ZERO);
		AddBit(m_pCurAnimNode->iCond, HP_ZERO);
	}


}

void CAnimController::SetGravity()
{
	if (CalBit(m_pCurAnimNode->iPreferences, IGNORE_GRAVITY, BIT_INCLUDE))
	{
		Rigidbody2D()->SetIgnGravity(true);
	}
	else
	{
		Rigidbody2D()->SetIgnGravity(false);
	}
}

void CAnimController::SetAttackCollider()
{
	int iIdx = m_pAttObj->GetLayerIdx();
	// 애니메이션이 오프셋을 가짐
	if (CalBit(m_pCurAnimNode->iPreferences, HAS_COLLIDER, BIT_INCLUDE))
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
			m_pAttObj->Collider2D()->SetOffsetPos(Vec2(vColOffset.x * (float)m_eCurAnimDir, vColOffset.y));
			m_pAttObj->Collider2D()->SetScale(vColScale);
		}
		else
		{
			m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * iIdx);
		}
	}
	else
	{
		m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * iIdx);
	}
}

void CAnimController::SetInvincible()
{
	const vector<CGameObject*>& vecChildObj = GetOwner()->GetChildObject();
	int iIdx =  m_pHitObj->GetLayerIdx();

	if (CalBit(m_pCurAnimNode->iPreferences, INVINCIBLE, BIT_INCLUDE))
	{
		m_pHitObj->Collider2D()->SetOffsetPos( EXPEL * iIdx );
	}
	else
	{
		m_pHitObj->Collider2D()->SetOffsetPos(Vec2::Zero);
	}

	// 피격시 몇초간 무적.
}

void CAnimController::PosChangeProgress()
{
	Vec2 vPosChange = m_pCurAnimNode->pAnim->GetPosChange();
	float vSumDuration = m_pCurAnimNode->pAnim->GetSumDuration();

	Vec2 vCurFramePosChange = vPosChange * DT / vSumDuration;
	Vec3 vPos =  Transform()->GetRelativePos();
	vPos.x += vCurFramePosChange.x * (float)m_eCurAnimDir;
	vPos.y += vCurFramePosChange.y;
	Transform()->SetRelativePos(vPos);
}



void CAnimController::SetComboProgress()
{
	if (KEY_TAP(KEY::LBTN) && CalBit(m_pCurAnimNode->iPreferences, COMBO_ANIM, BIT_INCLUDE))
	{
		m_bComboProgress = true;
	}
	else if(m_bComboProgress && m_pPrevAnimNode != m_pCurAnimNode)
	{
		m_bComboProgress = false;
	}
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
	m_pCurAnimNode = m_pNextNode;
	m_pNextNode = nullptr;
}

void CAnimController::Timer()
{
	CalDashTime();
}

void CAnimController::NodeProgress()
{
	// 콤보 지연
	static float fMemorizeTime = 1.f;
	static float fMemorizeAcc = 0.f;

	// 1초 동안 기억
	fMemorizeAcc += DT;
	if (fMemorizeAcc > fMemorizeTime || m_bPrevGround != m_bGround || m_bComboProgress)
		m_pReserveNode = nullptr;



	// 피격시 


	// ------------------------------------------------------------------------------------------------------------------

	// AnyState Node 분기
	for (size_t i = 0; i < m_pAnyStateNode->vecNextAnim.size(); ++i)
	{
		static UINT iCurAnyCond;
		iCurAnyCond = m_pCurAnimNode->iCond;

		// Exclude Bit 빼주기
		RemoveBit(iCurAnyCond, m_pAnyStateNode->vecNextAnim[i]->iExcludeCond);

		if (CalBit(iCurAnyCond, m_pAnyStateNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
		{
			m_pNextNode = mapAnimNode.find(m_pAnyStateNode->vecNextAnim[i]->pAnimKey)->second;
			return;
		}
	}

	// ------------------------------------------------------------------------------------------------------------------

	// Combo Progress
	if (CalBit(m_pCurAnimNode->iPreferences, HAS_RESERVE, BIT_INCLUDE))
	{
		static	bool IsAerial;
		if (CalBit(m_pCurAnimNode->iCond, GROUND, BIT_INCLUDE))
			IsAerial = false;
		else
			IsAerial = true;
		m_pReserveNode = m_pCurAnimNode->arrReserveAnim[IsAerial]; // 다음 콤보 애니메이션 저장
		m_pTmpSaveNode = m_pCurAnimNode;
		fMemorizeAcc = 0.f;
	}

	if (m_pReserveNode != nullptr)
	{
		if (!KEY_PRESSED(KEY::W) && !KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN) && m_pTmpSaveNode != m_pCurAnimNode)
		{
			m_pNextNode = mapAnimNode.find(m_pReserveNode->pAnimKey)->second;
			m_pReserveNode = nullptr;
			m_pTmpSaveNode = nullptr;
			return;
		}
	}

	//----------------------------------------------------------------------------------------------------------------------
	
	// Transition 비트 비교
	for (size_t i = 0; i < m_pCurAnimNode->vecNextAnim.size(); ++i)
	{
		static UINT iCurCond;
		iCurCond = m_pCurAnimNode->iCond;

		// Exclude Bit 빼주기
		RemoveBit(iCurCond, m_pCurAnimNode->vecNextAnim[i]->iExcludeCond);

		if (CalBit(iCurCond, m_pCurAnimNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
		{
			m_pNextNode = mapAnimNode.find(m_pCurAnimNode->vecNextAnim[i]->pAnimKey)->second;
			return;
		}
	}
}

void CAnimController::SetDir()
{
	// Uturn 방향전환
	if (CalBit(m_pCurAnimNode->iPreferences, DIR_CHANGE_ANIM, BIT_INCLUDE))
	{
		if (Animator2D()->FindAnimation(m_pCurAnimNode->pAnimKey)->IsFinish())
		{
			m_bDirChanging = true;
		}
	}
	// 저장된 애니메이션이 끝날 때
	else if (CalBit(m_pCurAnimNode->iPreferences, DIR_CHANGE_END, BIT_INCLUDE))
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_eNextDir = ANIM_DIR::ANIM_LEFT;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			m_eNextDir = ANIM_DIR::ANIM_RIGHT;
		}
	}
	// 공중 방향전환
	else if (!CalBit(m_pCurAnimNode->iCond, GROUND, BIT_INCLUDE))
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_eCurAnimDir = ANIM_DIR::ANIM_LEFT;
			Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		else if (KEY_PRESSED(KEY::D))
		{
			m_eCurAnimDir = ANIM_DIR::ANIM_RIGHT;
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}


}



void CAnimController::SetCurDir()
{
	// 한 프레임 나중에 방향 전환 Uturn
	if (m_bDirChanging)
	{
		if (m_eCurAnimDir == ANIM_DIR::ANIM_LEFT)
		{
			m_eCurAnimDir = ANIM_DIR::ANIM_RIGHT;
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
		else
		{
			m_eCurAnimDir = ANIM_DIR::ANIM_LEFT;
			Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		m_bDirChanging = false;
	}

	if (m_pPrevAnimNode != m_pCurAnimNode && m_eNextDir != ANIM_DIR::END)
	{
		m_eCurAnimDir = m_eNextDir;
		if (m_eCurAnimDir == ANIM_DIR::ANIM_LEFT)
		{
			Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		else
		{
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
		m_eNextDir = ANIM_DIR::END;
	}
}

void CAnimController::CalDashTime()
{
	static float fDashCool = 1.f;
	static float fDashAcc = 0.f;

	if (!m_bCanDash)
	{
		fDashAcc += DT;
		if (fDashCool <= fDashAcc)
		{
			fDashAcc = 0.f;
			m_bCanDash = true;
		}
	}
}

// 0번 -> Ground Reserve Anim  | 1번 -> Aerial Reserve Anim
void tAnimNode::SetReserve(const wstring& _pAnimPath, bool _bGroundAnim)
{
	if(_bGroundAnim)
		arrReserveAnim[0] = CAnimController::mapAnimNode.find(_pAnimPath)->second;
	else
		arrReserveAnim[1] = CAnimController::mapAnimNode.find(_pAnimPath)->second;
}
