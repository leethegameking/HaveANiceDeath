#include "pch.h"
#include "CPlayerScript.h"
#include "CPlayerMgr.h"

#include "CAnimController.h"

CPlayerScript::CPlayerScript()
	: CUnitScript(PLAYERSCRIPT)
{
	m_CurUnitInfo.m_eName = UNIT_NAME::PLAYER;
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CUnitScript(_origin)
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{
	CUnitScript::begin();

	m_pAnimCon = GetOwner()->GetScript<CAnimController>();
	CPlayerMgr::GetInst()->SetPlayerObj(GetOwner());

	// 시작 애니메이션 노드 초기화
	tAnimNode* pStartNode = CAnimController::mapAnimNode.find(L"animation\\player\\PlayerIdle.anim")->second;
	m_pAnimCon->SetCurAnimNode(pStartNode);
	Animator2D()->Play(pStartNode->pAnimKey);

	// AnyStateNode 초기화
	tAnimNode* pAnyStateNode = CAnimController::mapAnimNode.find(L"PlayerAnyNode")->second;
	m_pAnimCon->SetAnyStateNode(pAnyStateNode);

	float fAlpha = 0.f;
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fAlpha);
	Rigidbody2D()->SetIgnGravity(true);
	CPlayerMgr::GetInst()->SetPlayerDisable(true);

	tMtrlConst tMC = tMtrlConst{};
	GetOwner()->MeshRender()->GetCurMaterial()->SetConstMtrl(tMC);
}

void CPlayerScript::tick()
{
	CUnitScript::tick();
}


void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}



void CPlayerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
