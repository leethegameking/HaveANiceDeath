#include "pch.h"
#include "CEnemyController.h"

#include "CEnemyScript.h"
#include <Engine/CGameObject.h>

CEnemyController::CEnemyController()
	: CAnimController(SCRIPT_TYPE::ENEMYCONTROLLER)
{
}

CEnemyController::CEnemyController(const CEnemyController& _origin)
	: CAnimController(_origin)
{
}

CEnemyController::~CEnemyController()
{
}

void CEnemyController::begin()
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

void CEnemyController::tick()
{
	NodeProgress();
	SetPattern();
}

void CEnemyController::SetPattern()
{
	CEnemyScript* pEnemyScr = (CEnemyScript*)m_pUnitScr;
	pEnemyScr->SetPattern(m_pCurAnimNode->ePattern);
}

void CEnemyController::SetCondBit()
{
	// ANIM_FINISHED
	RemoveBit(m_pCurAnimNode->iCond, E_ANIM_FINISHED);
	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		AddBit(m_pCurAnimNode->iCond, E_ANIM_FINISHED);
	}
}

void CEnemyController::NodeProgress()
{
	// AnyStaeNode 분기
	if (m_pAnyStateNode)
	{
		for (size_t i = 0; i < m_pAnyStateNode->vecNextAnim.size(); ++i)
		{
			static UINT iCurAnyCond;
			iCurAnyCond = m_pCurAnimNode->iCond;

			// Exclude Bit 빼주기
			RemoveBit(iCurAnyCond, m_pAnyStateNode->vecNextAnim[i]->iExcludeCond);

			if (CalBit(iCurAnyCond, m_pAnyStateNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
			{
				tAnimNode* pNextNode = mapAnimNode.find(m_pAnyStateNode->vecNextAnim[i]->pAnimKey)->second;
				Animator2D()->Play(m_pAnyStateNode->vecNextAnim[i]->pAnimKey, CalBit(pNextNode->iPreferences, REPEAT, BIT_INCLUDE));
				m_pCurAnimNode = pNextNode;
				return;
			}
		}
	}

	// Transition 비트 비교
	for (size_t i = 0; i < m_pCurAnimNode->vecNextAnim.size(); ++i)
	{
		static UINT iCurCond;
		iCurCond = m_pCurAnimNode->iCond;

		// Exclude Bit 빼주기
		RemoveBit(iCurCond, m_pCurAnimNode->vecNextAnim[i]->iExcludeCond);

		if (CalBit(iCurCond, m_pCurAnimNode->vecNextAnim[i]->iTranCond, BIT_EQUAL))
		{
			tAnimNode* pNextNode = mapAnimNode.find(m_pCurAnimNode->vecNextAnim[i]->pAnimKey)->second;
			Animator2D()->Play(m_pCurAnimNode->vecNextAnim[i]->pAnimKey, CalBit(pNextNode->iPreferences, REPEAT, BIT_INCLUDE));
			m_pCurAnimNode = pNextNode;
			return;
		}
	}
}

