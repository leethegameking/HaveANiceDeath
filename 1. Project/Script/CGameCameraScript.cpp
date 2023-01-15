#include "pch.h"
#include "CGameCameraScript.h"

#include <Engine/CDevice.h>
#include "CPlayerMgr.h"
#include "CCameraMgr.h"

// == ÇÑ±¹¾î
CGameCameraScript::CGameCameraScript()
	: CScript(GAMECAMERASCRIPT)
	, m_fCamSpeed(100.f)
	, m_pPlayer(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Cam Speed ", &m_fCamSpeed);
}

CGameCameraScript::CGameCameraScript(int _iScriptType)
	: CScript(_iScriptType)
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

void CGameCameraScript::begin()
{
	CCameraMgr::GetInst()->RegisterCamera(GetOwner());
}

void CGameCameraScript::tick()
{
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



