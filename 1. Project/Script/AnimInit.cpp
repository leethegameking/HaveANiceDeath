#include "pch.h"
#include "CAnimController.h"

#include <Engine/CResMgr.h>

map<wstring, tAnimNode*> CAnimController::mapPlayerNode;
map<wstring, tAnimNode*> CAnimController::mapEnemyNode;

void CAnimController::AnimConInit()
{
	CreatePlayerAnimCon();
}


#define FindPlayerNode(AnimPath) mapPlayerNode.find(L#AnimPath)->second
void CAnimController::CreatePlayerAnimCon()
{
	
	tAnimNode* pPlayerNode = nullptr;
	map<wstring, Ptr<CRes>> mapAnim = CResMgr::GetInst()->GetResource(RES_TYPE::ANIMATION2D);

	// 이름으로 파싱해 Player Animation Key만 넣어서 생성
	map<wstring, Ptr<CRes>>::iterator iter = mapAnim.begin();
	for (; iter != mapAnim.end(); ++iter)
	{
		wstring strAnimKey = iter->second->GetKey();
		if (strAnimKey.rfind(L"player") != -1)
		{
			pPlayerNode = new tAnimNode;
			pPlayerNode->pAnimKey = strAnimKey;
			mapPlayerNode.insert({ strAnimKey, pPlayerNode });
		}
	}

	tAnimNode* pAnimNode = FindPlayerNode(animation\\player\\PlayerIdle.anim);
	pAnimNode->vecNextAnim.push_back(FindPlayerNode(animation\\player\\PlayerIdleToRun.anim));	pAnimNode->vecEnsureFinish.push_back(false);
	pAnimNode->vecNextAnim.push_back(FindPlayerNode(animation\\player\\PlayerIdleUturn.anim));	pAnimNode->vecEnsureFinish.push_back(false);
	pAnimNode->vecNextAnim.push_back(FindPlayerNode(animation\\player\\PlayerJumpDown.anim));	pAnimNode->vecEnsureFinish.push_back(false);
	pAnimNode->vecNextAnim.push_back(FindPlayerNode(animation\\player\\PlayerIdle.anim));		pAnimNode->vecEnsureFinish.push_back(true);

	AddBit(pAnimNode->iCond, GROUND);

	pAnimNode = FindPlayerNode(animation\\player\\PlayerIdleToRun.anim);

}

void CAnimController::DelAnimConMap()
{
	Safe_Del_Map<wstring, tAnimNode*>(mapPlayerNode);
	Safe_Del_Map<wstring, tAnimNode*>(mapEnemyNode);
}