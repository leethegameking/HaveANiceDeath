#include "pch.h"
#include "CGameCameraScript.h"

#include <Engine/CDevice.h>
#include "CPlayerMgr.h"

// == ÇÑ±¹¾î
CGameCameraScript::CGameCameraScript()
	: CScript(GAMECAMERASCRIPT)
	, m_fCamSpeed(100.f)
	, m_pPlayer(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Cam Speed ", &m_fCamSpeed);
}

CGameCameraScript::CGameCameraScript(const CGameCameraScript& _origin)
	: CScript(_origin)
	, m_fCamSpeed(100.f)
	, m_pPlayer(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Cam Speed ", &m_fCamSpeed);
}

CGameCameraScript::~CGameCameraScript()
{
}

void CGameCameraScript::tick()
{
	if(!m_pPlayer)
		m_pPlayer = CPlayerMgr::GetInst()->GetPlayerObj();
	else
	{
		Vec3 vPlayerPos = m_pPlayer->Transform()->GetRelativePos();
		Vec3 vCamPos = Transform()->GetRelativePos();
		vPlayerPos.y += 50.f;
		Vec3 vPosDiff = vPlayerPos - vCamPos;
		Transform()->AddRelativePos(vPosDiff / 10.f * (DT / 0.025f));
	}

	

	//if (!CDevice::GetInst()->IsWindowFocused())
	//	return;

	//Vec3 vCamPos = Transform()->GetRelativePos();
	//float fCamSpeed = m_fCamSpeed;

	//if (KEY_PRESSED(KEY::LSHIFT))
	//{
	//	fCamSpeed *= 5.f;
	//}

	//if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::LCTRL))
	//{
	//	vCamPos.y += fCamSpeed * DT;
	//}
	//if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::LCTRL))
	//{
	//	vCamPos.y -= fCamSpeed * DT;
	//}
	//if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::LCTRL))
	//{
	//	vCamPos.x -= fCamSpeed * DT;
	//}
	//if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::LCTRL))
	//{
	//	vCamPos.x += fCamSpeed * DT;
	//}

	//Transform()->SetRelativePos(vCamPos);
}

void CGameCameraScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_fCamSpeed, sizeof(float), 1, _pFile);
}

void CGameCameraScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_fCamSpeed, sizeof(float), 1, _pFile);
}



