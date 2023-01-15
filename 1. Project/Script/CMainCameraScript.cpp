#include "pch.h"
#include "CMainCameraScript.h"
#include "CPlayerMgr.h"

CMainCameraScript::CMainCameraScript()
	: CGameCameraScript((int)SCRIPT_TYPE::MAINCAMERASCRIPT)
{
	m_eType = CAMERA_TYPE::MAIN;
}

CMainCameraScript::CMainCameraScript(const CMainCameraScript& _origin)
	: CGameCameraScript(_origin)
{
	m_eType = CAMERA_TYPE::MAIN;
}

CMainCameraScript::~CMainCameraScript()
{
}

void CMainCameraScript::begin()
{
	CGameCameraScript::begin();
}

void CMainCameraScript::tick()
{
	if (!m_pPlayer)
		m_pPlayer = CPlayerMgr::GetInst()->GetPlayerObj();
	else
	{
		Vec3 vPlayerPos = m_pPlayer->Transform()->GetRelativePos();
		Vec3 vCamPos = Transform()->GetRelativePos();
		vPlayerPos.y += 50.f;
		Vec3 vPosDiff = vPlayerPos - vCamPos;
		Transform()->AddRelativePos(vPosDiff / 10.f * (DT / 0.025f));
	}
}

void CMainCameraScript::SaveToFile(FILE* _pFile)
{
	CGameCameraScript::SaveToFile(_pFile);
}

void CMainCameraScript::LoadFromFile(FILE* _pFile)
{
	CGameCameraScript::LoadFromFile(_pFile);
}



