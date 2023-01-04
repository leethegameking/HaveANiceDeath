#include "pch.h"
#include "CAnimController.h"

#include <Engine/CResMgr.h>

map<wstring, tAnimNode*> CAnimController::mapAnimNode;

void CAnimController::AnimConInit()
{
	CreatePlayerAnimCon();
}


#define FindNode(AnimPath) mapAnimNode.find(L#AnimPath)->second
#define PushTranNode(AnimPath) 	pTrNode = new tTransitionNode(L#AnimPath); \
								pNode->vecNextAnim.push_back(pTrNode)
void CAnimController::CreatePlayerAnimCon()
{
	
	tAnimNode* pPlayerNode = nullptr;
	map<wstring, Ptr<CRes>> mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);

	// 이름으로 파싱해 Player Animation Key만 넣어서 생성
	map<wstring, Ptr<CRes>>::iterator iter = mapAnim.begin();
	for (; iter != mapAnim.end(); ++iter)
	{
		pPlayerNode = new tAnimNode;
		pPlayerNode->pAnim = (CAnimation2D*)iter->second.Get();
		pPlayerNode->pAnimKey = pPlayerNode->pAnim->GetKey();
		mapAnimNode.insert({ pPlayerNode->pAnimKey, pPlayerNode });
	}


	tAnimNode* pNode = nullptr;
	tTransitionNode* pTrNode = nullptr;

	pNode = FindNode(animation\\player\\PlayerIdle.anim);  pNode->bNeedDirChange = true;
	PushTranNode(animation\\player\\PlayerIdle.anim);
	AddBit(pTrNode->iTranCond, GROUND | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerIdleToRun.anim);
	AddBit(pTrNode->iTranCond, GROUND | KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerIdleUturnR.anim);
	AddBit(pTrNode->iTranCond, GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);


	pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	PushTranNode(animation\\player\\PlayerRun.anim);
	AddBit(pTrNode->iTranCond, GROUND | KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	AddBit(pTrNode->iTranCond, GROUND);

	pNode = FindNode(animation\\player\\PlayerRun.anim); pNode->bNeedDirChange = true;
	PushTranNode(animation\\player\\PlayerRun.anim);
	AddBit(pTrNode->iTranCond, GROUND | KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	AddBit(pTrNode->iTranCond, GROUND);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	AddBit(pTrNode->iTranCond, GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);

	pNode = FindNode(animation\\player\\PlayerRunToIdle.anim);
	PushTranNode(animation\\player\\PlayerIdle.anim);

	pNode = FindNode(animation\\player\\PlayerIdleUturnR.anim); pNode->bDirChangeAnim = true; pNode->bNoMove = true;
	PushTranNode(animation\\player\\PlayerIdle.anim);

	pNode = FindNode(animation\\player\\PlayerRunUturnR.anim); pNode->bDirChangeAnim = true;
	PushTranNode(animation\\player\\PlayerRun.anim);
}

void CAnimController::DelAnimConMap()
{
	map<wstring, tAnimNode*>::iterator iter = mapAnimNode.begin();
	for (; iter != mapAnimNode.end(); ++iter)
	{
		vector<tTransitionNode*>& vecTrNode = iter->second->vecNextAnim;
		Safe_Del_Vec(vecTrNode);
	}
	Safe_Del_Map<wstring, tAnimNode*>(mapAnimNode);
}

