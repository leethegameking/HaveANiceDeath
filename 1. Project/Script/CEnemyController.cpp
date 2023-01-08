#include "pch.h"
#include "CEnemyController.h"

void CEnemyController::begin()
{
	// 각 ObjScript에서 해주게 변경.
	m_pCurAnimNode = mapAnimNode.find(L"animation\\player\\EWorkmanWaiting.anim")->second;
	Animator2D()->Play(m_pCurAnimNode->pAnimKey);

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
	m_pPrevAnimNode = m_pCurAnimNode;
	if (m_pNextNode != nullptr)
	{
		PlayNextNode();
	}

}

CEnemyController::CEnemyController()
	: CAnimController(SCRIPT_TYPE::ENEMYCONTROLLER)
{
}

CEnemyController::~CEnemyController()
{
}
