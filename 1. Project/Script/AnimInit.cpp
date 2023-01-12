#include "pch.h"
#include "CAnimController.h"

#include <Engine/CResMgr.h>

map<wstring, tAnimNode*> CAnimController::mapAnimNode;

void CAnimController::AnimConInit()
{
	CreateMapAnimNode();

	AddPlayerAnyNode();
	CreatePlayerAnimCon();
}

void CAnimController::DelAnimConMap()
{
	map<wstring, tAnimNode*>::iterator iter = mapAnimNode.begin();
	for (; iter != mapAnimNode.end(); ++iter)
	{
		Safe_Del_Vec(iter->second->vecNextAnim);
	}

	Safe_Del_Map(mapAnimNode);
}

void CAnimController::CreateMapAnimNode()
{
	tAnimNode* pAnimNode = nullptr;

	map<wstring, Ptr<CRes>> mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);

	// 이름으로 파싱해 Player Animation Key만 넣어서 생성
	map<wstring, Ptr<CRes>>::iterator iter = mapAnim.begin();
	for (; iter != mapAnim.end(); ++iter)
	{
		pAnimNode = new tAnimNode;
		pAnimNode->pAnim = (CAnimation2D*)iter->second.Get();
		pAnimNode->pAnimKey = pAnimNode->pAnim->GetKey();
		mapAnimNode.insert({ pAnimNode->pAnimKey, pAnimNode });
	}
}

void CAnimController::CreatePlayerAnimCon()
{
	tAnimNode* pNode = nullptr;
	tTransitionNode* pTrNode = nullptr;
	// ============================== Dash ==============================
	pNode = FindNode(animation\\player\\PlayerDash.anim); pNode->AddPreferences( DIR_CHANGE_END | ANY_STATE | NO_MOVE | IGNORE_GRAVITY | INVINCIBLE );
	PushTranNode(animation\\player\\PlayerIdle.anim); pTrNode->AddInclude(ANIM_FINISHED);
	
	// ============================== Ground ==============================
	pNode = FindNode(animation\\player\\PlayerIdle.anim); pNode->AddPreferences(NEED_DIR_CHANGE | REPEAT);
	PushTranNode(animation\\player\\PlayerIdleToRun.anim);
	pTrNode->AddInclude(GROUND | KEY_A_OR_D, ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerIdleUturnR.anim);
	pTrNode->AddInclude(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddInclude(SPEED_Y_POSITIVE);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddInclude(SPEED_Y_NEGATIVE);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerIdleToRun.anim); 	pNode->AddPreferences(NEED_DIR_CHANGE);
	PushTranNode(animation\\player\\PlayerRun.anim);
	pTrNode->AddInclude(GROUND | KEY_A_OR_D | ANIM_FINISHED, ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddInclude(GROUND, KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddInclude(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddInclude(SPEED_Y_POSITIVE);	
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddInclude(SPEED_Y_NEGATIVE);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRun.anim); pNode->AddPreferences(NEED_DIR_CHANGE | REPEAT);
	PushTranNode(animation\\player\\PlayerRunUturnR.anim);
	pTrNode->AddInclude(GROUND | KEY_A_OR_D | ANIM_DIR_CHANGED);
	PushTranNode(animation\\player\\PlayerJumpStart.anim);
	pTrNode->AddInclude(SPEED_Y_POSITIVE);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddInclude(SPEED_Y_NEGATIVE);
	PushTranNode(animation\\player\\PlayerRunToIdle.anim);
	pTrNode->AddInclude(GROUND, KEY_A_OR_D);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRunToIdle.anim); /*pNode->AddPreferences(NO_MOVE);*/
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerIdleUturnR.anim);	pNode->AddPreferences(DIR_CHANGE_ANIM /*| NO_MOVE*/);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerRunUturnR.anim); pNode->AddPreferences(DIR_CHANGE_ANIM /*| NO_MOVE*/);
	PushTranNode(animation\\player\\PlayerRun.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(GROUND | MOUSE_LEFT);

	// ============================== Jump ==============================
	pNode = FindNode(animation\\player\\PlayerJumpStart.anim);
	PushTranNode(animation\\player\\PlayerFalling.anim);
	pTrNode->AddInclude(SPEED_Y_NEGATIVE);
	PushTranNode(animation\\player\\PlayerAirCombo1.anim);
	pTrNode->AddInclude(MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerFalling.anim); pNode->AddPreferences(REPEAT);
	PushTranNode(animation\\player\\PlayerLanding.anim);
	pTrNode->AddInclude(GROUND);
	PushTranNode(animation\\player\\PlayerAirCombo1.anim);
	pTrNode->AddInclude(MOUSE_LEFT);

	pNode = FindNode(animation\\player\\PlayerLanding.anim); pNode->AddPreferences(NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== Attack ==============================
	pNode = FindNode(animation\\player\\PlayerFightToIdle.anim); pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerFightToIdle2.anim); pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo1.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerCombo2.anim);
	pTrNode->AddInclude(ANIM_FINISHED | GROUND | COMBO_PROGRESS); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerAirCombo2.anim);
	pTrNode->AddInclude(ANIM_FINISHED | COMBO_PROGRESS); pNode->SetReserve(pTrNode->pAnimKey, AERIALANIM);
	PushTranNode(animation\\player\\PlayerFightToIdle2.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo2.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER);
	PushTranNode(animation\\player\\PlayerCombo3.anim);
	pTrNode->AddInclude(ANIM_FINISHED | GROUND | COMBO_PROGRESS); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerFightToIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo3.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER);
	PushTranNode(animation\\player\\PlayerCombo4.anim);
	pTrNode->AddInclude(ANIM_FINISHED | GROUND | COMBO_PROGRESS); pNode->SetReserve(pTrNode->pAnimKey);
	PushTranNode(animation\\player\\PlayerFightToIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo4.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM | HAS_COLLIDER);
	PushTranNode(animation\\player\\PlayerCombo4_2.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerCombo4_2.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM | HAS_COLLIDER);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerJumpAttack.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | HAS_COLLIDER);
	PushTranNode(animation\\player\\PlayerIdle.anim);
	pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== JumpAttack ==============================
	pNode = FindNode(animation\\player\\PlayerAirCombo1.anim);  pNode->AddPreferences(DIR_CHANGE_END | HAS_RESERVE | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerCombo1.anim);
	pTrNode->AddInclude(ANIM_FINISHED | COMBO_PROGRESS, GROUND); pNode->SetReserve(pTrNode->pAnimKey, AERIALANIM); 
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED); 

	pNode = FindNode(animation\\player\\PlayerAirCombo2.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM | HAS_COLLIDER | IGNORE_GRAVITY);
	PushTranNode(animation\\player\\PlayerIdle.anim); 
	pTrNode->AddInclude(ANIM_FINISHED);

	// ============================== Hit ==============================
	pNode = FindNode(animation\\player\\PlayerHit.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM | INVINCIBLE_START);
	PushTranNode(animation\\player\\PlayerIdle.anim);
	pTrNode->AddInclude(ANIM_FINISHED);

	pNode = FindNode(animation\\player\\PlayerDisappear.anim);  pNode->AddPreferences(DIR_CHANGE_END | NO_MOVE | COMBO_ANIM);
}

void CAnimController::AddPlayerAnyNode()
{
	tAnimNode* pNode = new tAnimNode;
	tTransitionNode* pTrNode = nullptr;
	mapAnimNode.insert({ L"PlayerAnyNode", pNode });
	
	PushTranNode(animation\\player\\PlayerDash.anim);
	pTrNode->AddInclude(KEY_SHIFT | CAN_DASH);

	PushTranNode(animation\\player\\PlayerHit.anim);
	pTrNode->AddInclude(HP_DOWN);

	PushTranNode(animation\\player\\PlayerDisappear.anim);
	pTrNode->AddInclude(HP_ZERO);

	PushTranNode(animation\\player\\PlayerJumpAttack.anim);
	pTrNode->AddInclude(MOUSE_LEFT | KEY_W | CAN_JUMP_ATTACK);
}

