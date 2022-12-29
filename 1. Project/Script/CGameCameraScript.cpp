#include "pch.h"
#include "CGameCameraScript.h"

#include <Engine/CDevice.h>

CGameCameraScript::CGameCameraScript()
	: CScript(GAMECAMERASCRIPT)
	, m_fCamSpeed(10.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Cam Speed ", &m_fCamSpeed);
}

CGameCameraScript::~CGameCameraScript()
{
}

void CGameCameraScript::tick()
{
	if (!CDevice::GetInst()->IsWindowFocused())
		return;

	Vec3 vCamPos = Transform()->GetRelativePos();
	float fCamSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		fCamSpeed *= 5.f;
	}

	if (KEY_PRESSED(KEY::W))
	{
		vCamPos.y += fCamSpeed * DT;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCamPos.y -= fCamSpeed * DT;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCamPos.x -= fCamSpeed * DT;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCamPos.x += fCamSpeed * DT;
	}

	Transform()->SetRelativePos(vCamPos);
}

void CGameCameraScript::SaveToFile(FILE* _pFile)
{
}

void CGameCameraScript::LoadFromFile(FILE* _pFile)
{
}



