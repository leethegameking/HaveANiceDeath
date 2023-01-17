#include "pch.h"
#include "CBossCameraScript.h"

CBossCameraScript::CBossCameraScript()
	: CGameCameraScript((int)SCRIPT_TYPE::BOSSCAMERASCRIPT)
{
	m_eType = CAMERA_TYPE::BOSS;
}

CBossCameraScript::CBossCameraScript(const CBossCameraScript& _origin)
	: CGameCameraScript(_origin)
{
	m_eType = CAMERA_TYPE::BOSS;
}

CBossCameraScript::~CBossCameraScript()
{
}

void CBossCameraScript::begin()
{
	CGameCameraScript::begin();
}

void CBossCameraScript::tick()
{

}

void CBossCameraScript::SaveToFile(FILE* _pFile)
{
	CGameCameraScript::SaveToFile(_pFile);
}

void CBossCameraScript::LoadFromFile(FILE* _pFile)
{
	CGameCameraScript::LoadFromFile(_pFile);
}


