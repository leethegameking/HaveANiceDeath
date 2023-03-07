#include "pch.h"
#include "CTriggerScript.h"
#include "CPlayerMgr.h"
#include "CTriggerMgr.h"
#include "CCameraMgr.h"
#include "CMainCameraScript.h"

void CTriggerScript::PlayerGo()
{
	static CGameObject* pPlayer;
	static float fSpeed = 500.f;

	if (m_bFirst)
	{
		pPlayer = CPlayerMgr::GetInst()->GetPlayerObj();
		pPlayer->Animator2D()->Play(L"animation\\player\\PlayerRun.anim");
		pPlayer->Rigidbody2D()->SetIgnGravity(false);
		CPlayerMgr::GetInst()->SetPlayerDisable(true);
		m_bFirst = false;
	}

	pPlayer->Transform()->AddRelativePos(Vec3(fSpeed * DT, 0.f, 0.f));
}

void CTriggerScript::PlayerStop()
{
	static CGameObject* pPlayer;
	if (m_bFirst)
	{
		pPlayer = CPlayerMgr::GetInst()->GetPlayerObj();
		pPlayer->Animator2D()->Play(L"animation\\player\\PlayerRunToIdle.anim", false);
		m_bFirst = false;
	}

	if (pPlayer->Animator2D()->GetCurAnim()->IsFinish())
	{
		pPlayer->Animator2D()->Play(L"animation\\player\\PlayerIdle.anim");
		CCameraMgr::GetInst()->GetMainCamera()->GetScript<CMainCameraScript>()->SetMainCamState(MAIN_CAM_STATE::BOSS_BRAD);

		CTriggerMgr::GetInst()->DestroyTriggerObj(TRIGGER_TYPE::PLAYER_STOP);
	}
}
