#include "pch.h"
#include "CCutSceneCameraScript.h"

CCutSceneCameraScript::CCutSceneCameraScript()
	: CGameCameraScript((int)SCRIPT_TYPE::CUTSCENECAMERASCRIPT)
{
	m_eType = CAMERA_TYPE::CUT_SCENE;
}

CCutSceneCameraScript::CCutSceneCameraScript(const CCutSceneCameraScript& _origin)
	: CGameCameraScript(_origin)
{
	m_eType = CAMERA_TYPE::CUT_SCENE;
}

CCutSceneCameraScript::~CCutSceneCameraScript()
{
}

void CCutSceneCameraScript::begin()
{
	CGameCameraScript::begin();
}

void CCutSceneCameraScript::tick()
{
}

void CCutSceneCameraScript::SaveToFile(FILE* _pFile)
{
	CGameCameraScript::SaveToFile(_pFile);
}

void CCutSceneCameraScript::LoadFromFile(FILE* _pFile)
{
	CGameCameraScript::LoadFromFile(_pFile);
}



