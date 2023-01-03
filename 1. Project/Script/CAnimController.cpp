#include "pch.h"
#include "CAnimController.h"

#include "CUnitScript.h"

CAnimController::CAnimController()
	: CScript(int(SCRIPT_TYPE::ANIMCONTROLLER))
	, m_ObjType(OBJ_PLAYER)
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
		m_pAnimNode = mapPlayerNode.find(L"animation\\player\\PlayerIdle.anim")->second;
		Animator2D()->Play(m_pAnimNode->pAnimKey);
	}
}

void CAnimController::tick()
{
	GetCondBit();

	UINT iCond = m_pAnimNode->iCond;
	vector<tAnimNode*>& vecNextNode = m_pAnimNode->vecNextAnim;
	for (size_t i = 0; i < vecNextNode.size(); ++i)
	{
		// ANIM_CONDITON Bit 
		if (CalBit(vecNextNode[i]->iCondCmp, iCond, BIT_EQUAL))
		{
			// Ensure Finish
			if (m_pAnimNode->vecEnsureFinish[i])
			{
				if(m_pAnimNode->IsFinished)
					Animator2D()->Play(vecNextNode[i]->pAnimKey, false);
			}
			else
			{
				Animator2D()->Play(vecNextNode[i]->pAnimKey, false);
			}
		}
	}
}

void CAnimController::GetCondBit()
{
	m_pAnimNode->iCondCmp = 0;
	if (Rigidbody2D()->IsGround())
		AddBit(m_pAnimNode->iCondCmp, GROUND);

	if (Animator2D()->GetCurAnim()->IsFinish())
		m_pAnimNode->IsFinished = true;
	else
		m_pAnimNode->IsFinished = false;
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




