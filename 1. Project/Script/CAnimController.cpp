#include "pch.h"
#include "CAnimController.h"

#include "CUnitScript.h"

CAnimController::CAnimController()
	: CScript(int(SCRIPT_TYPE::ANIMCONTROLLER))
	, m_ObjType(OBJ_PLAYER)
	, m_eAnimDir(ANIM_DIR::ANIM_RIGHT)
	, m_bDirChanged(false)
{
	//m_mapAnim = Animator2D()->GetAnimMap();

	//m_vecAnimName.reserve(m_mapAnim.size());
	//
	//map<wstring, Ptr<CAnimation2D>>::iterator iter = m_mapAnim.begin();
	//for (; iter != m_mapAnim.end(); ++iter)
	//{
	//	m_vecAnimName.push_back(iter->first);
	//	m_vecAnim.push_back(iter->second);
	//}

	// Unit 상속받은 Player Enemy 스크립트 완성되면 Type분기처리
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
		if (m_pAnimNode->vecNextAnim[i]->iTranCond == 0 && CalBit(m_pAnimNode->iCond, ANIM_FINISHED, BIT_FUNC_OPT::BIT_LEAST_ONE))
		{
			Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
			m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
			break;
		}

		if (CalBit(m_pAnimNode->iCond, m_pAnimNode->vecNextAnim[i]->iTranCond, BIT_FUNC_OPT::BIT_EQUAL))
		{
			Animator2D()->Play(m_pAnimNode->vecNextAnim[i]->pAnimKey, false);
			m_pAnimNode = mapAnimNode.find(m_pAnimNode->vecNextAnim[i]->pAnimKey)->second;
			break;
		}
	}
	
	static bool bOneFrameAfter = false;
	if (m_pPrevAnimNode->bDirChangeAnim && Animator2D()->FindAnimation(m_pPrevAnimNode->pAnimKey)->IsFinish())
	{
		bOneFrameAfter = true;
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
	m_bDirChanged = false;

	if (Rigidbody2D()->IsGround())
		AddBit(m_pAnimNode->iCond, GROUND);

	if (Animator2D()->GetCurAnim()->OFAFinish())
		AddBit(m_pAnimNode->iCond, ANIM_FINISHED);

	if( KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D) )
		AddBit(m_pAnimNode->iCond, KEY_A_OR_D);


	if (KEY_PRESSED(KEY::A))
	{
		if (m_eAnimDir != ANIM_DIR::ANIM_LEFT)
		{
			if (m_pAnimNode->bNeedDirChange)
			{
				AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
				m_bDirChanged = true;
			}
		}
	}

	if (KEY_PRESSED(KEY::D))
	{
		if (m_eAnimDir != ANIM_DIR::ANIM_RIGHT)
		{
			if (m_pAnimNode->bNeedDirChange)
			{
				AddBit(m_pAnimNode->iCond, ANIM_DIR_CHANGED);
				m_bDirChanged = true;
			}
		}
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




