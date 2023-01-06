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
								assert(pTrNode->pAnim.Get()); \
								pNode->vecNextAnim.push_back(pTrNode)
#define AERIALANIM false


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
	// ============================== Dash ==============================
	pNode = FindNode(animation\\player\\PlayerDash.anim); pNode->AddPreferences( DIR_CHANGE_END | DASH_ANIM | NO_MOVE | IGNORE_GRAVITY | INVINCIBLE );
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);
	
	// ============================== Ground ==============================
	pNode = FindNode(animation\\player\\PlayerIdle.anim); pNode->AddPreferences(NEED_DIR_CHANGE | REPEAT);
	PushTranNode(animation\\player\\PlayerIdleToRun.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerIdleUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddTran(GROUND_TO_AERIAL | SPEED_Y_POSITIVE);			pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_NEGATIVE);									pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerIdleToRun.anim); 	pNode->AddPreferences(NEED_DIR_CHANGE);
	PushTranNode(animation\\player\\PlayerRun.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddTran(GROUND);									pTrNode->AddExclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddInclude(GROUND_TO_AERIAL | SPEED_Y_POSITIVE);	
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_NEGATIVE);									pTrNode->AddExclude(KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRun.anim); pNode->AddPreferences(NEED_DIR_CHANGE | REPEAT);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddTran(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);		pTrNode->AddExclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddInclude(GROUND_TO_AERIAL | SPEED_Y_POSITIVE);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_NEGATIVE);									pTrNode->AddExclude(KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddTran(GROUND);										pTrNode->AddExclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRunToIdle.anim); /*pNode->AddPreferences(NO_MOVE);*/
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerIdleUturnR.anim);	pNode->AddPreferences(DIR_CHANGE_ANIM /*| NO_MOVE*/);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRunUturnR.anim); pNode->AddPreferences(DIR_CHANGE_ANIM /*| NO_MOVE*/);
	PushTranNode(animation\\player\\PlayerRun.anim); pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	// ============================== Jump ==============================
	pNode = FindNode(animation\\player\\PlayerJumpStart.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(SPEED_Y_NEGATIVE);									pTrNode->AddExclude(KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerAirCombo1.anim);
	pTrNode->AddInclude(MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerFalling.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddTran(ANIM_FINISHED);								pTrNode->AddExclude(KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerLanding.anim);
	pTrNode->AddTran(GROUND);										pTrNode->AddExclude(KEY_A_OR_D | ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerAirCombo1.anim);
	pTrNode->AddInclude(MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerLanding.anim); pNode->AddPreferences(NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== Attack ==============================
	pNode = FindNode(animation\\player\\PlayerFightToIdle.anim); pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerFightToIdle2.anim); pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo1.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerCombo2.anim);
	pTrNode->AddTran(ANIM_FINISHED | GROUND | COMBO_PROGRESS);		pTrNode->AddExclude(KEY_A_OR_D | MOUSE_LEFT); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerAirCombo2.anim);
	pTrNode->AddTran(ANIM_FINISHED | COMBO_PROGRESS);		pTrNode->AddExclude(KEY_A_OR_D | MOUSE_LEFT); pNode->SetReserve(pTrNode->pAnimKey, AERIALANIM);
	PushTranNode(animation\\player\\PlayerFightToIdle2.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo2.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM);
	PushTranNode(animation\\player\\PlayerCombo3.anim);
	pTrNode->AddTran(ANIM_FINISHED | GROUND | COMBO_PROGRESS);		pTrNode->AddExclude(KEY_A_OR_D | MOUSE_LEFT); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerFightToIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo3.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM);
	PushTranNode(animation\\player\\PlayerCombo4.anim);
	pTrNode->AddTran(ANIM_FINISHED | GROUND | COMBO_PROGRESS);		pTrNode->AddExclude(KEY_A_OR_D | MOUSE_LEFT); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerFightToIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo4.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM);
	PushTranNode(animation\\player\\PlayerCombo4_2.anim); pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo4_2.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== JumpAttack ==============================
	pNode = FindNode(animation\\player\\PlayerAirCombo1.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(ANIM_FINISHED | COMBO_PROGRESS); pNode->SetReserve(pTrNode->pAnimKey, AERIALANIM); 
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED); 

	pNode = FindNode(animation\\player\\PlayerAirCombo2.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== Hit ==============================
	pNode = FindNode(animation\\player\\PlayerHit.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM);


	pNode = FindNode(animation\\player\\PlayerDisappear.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM);
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

