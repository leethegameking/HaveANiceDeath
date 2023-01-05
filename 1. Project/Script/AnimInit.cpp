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

	// ============================== Ground ==============================
	pNode = FindNode(animation\\player\\PlayerIdle.anim); pNode->AddPreferences(NEED_DIR_CHANGE);
	PushTranNode(animation\\player\\PlayerIdle.anim);
	pTrNode->AddTran(GROUND | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerIdleToRun.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerIdleUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddTran(GROUND_TO_AERIAL | SPEED_Y_POSITIVE );			pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_TURN);									pTrNode->AddExclude(KEY_A_OR_D);

	pNode = FindNode(animation\\player\\PlayerIdleToRun.anim); 	pNode->AddPreferences(NEED_DIR_CHANGE);
	PushTranNode(animation\\player\\PlayerRun.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddTran(GROUND);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddTran(GROUND_TO_AERIAL | SPEED_Y_POSITIVE);			pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_TURN);									pTrNode->AddExclude(KEY_A_OR_D);

	pNode = FindNode(animation\\player\\PlayerRun.anim); pNode->AddPreferences(NEED_DIR_CHANGE);
	PushTranNode(animation\\player\\PlayerRun.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddTran(GROUND);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddTran(GROUND_TO_AERIAL | SPEED_Y_POSITIVE);			pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_TURN);								pTrNode->AddExclude(KEY_A_OR_D);

	pNode = FindNode(animation\\player\\PlayerRunToIdle.anim); pNode->AddPreferences(NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim);

	pNode = FindNode(animation\\player\\PlayerIdleUturnR.anim);	pNode->AddPreferences(DIR_CHANGE_ANIM | NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim);

	pNode = FindNode(animation\\player\\PlayerRunUturnR.anim); pNode->AddPreferences(DIR_CHANGE_ANIM | NO_MOVE);
	PushTranNode(animation\\player\\PlayerRun.anim);

	// ============================== Jump ==============================
	pNode = FindNode(animation\\player\\PlayerJumpStart.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_TURN);									pTrNode->AddExclude(KEY_A_OR_D | ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerFalling.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(ANIM_FINISHED);								pTrNode->AddExclude(KEY_A_OR_D );
	PushTranNode(animation\\player\\PlayerLanding.anim);
	pTrNode->AddTran(GROUND | AERIAL_TO_GROUND);					pTrNode->AddExclude(KEY_A_OR_D);

	pNode = FindNode(animation\\player\\PlayerLanding.anim); pNode->AddPreferences(NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim);
	
	// ============================== Attack ==============================
	pNode = FindNode(animation\\player\\PlayerJumpStart.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);


	//pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	//PushTranNode(animation\\player\\PlayerRun.anim);

	//pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	//PushTranNode(animation\\player\\PlayerRun.anim);

	//pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	//PushTranNode(animation\\player\\PlayerRun.anim);

	//pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	//PushTranNode(animation\\player\\PlayerRun.anim);

	//pNode = FindNode(animation\\player\\PlayerIdleToRun.anim);
	//PushTranNode(animation\\player\\PlayerRun.anim);

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

