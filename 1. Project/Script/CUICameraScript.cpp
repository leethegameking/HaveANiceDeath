#include "pch.h"
#include "CUICameraScript.h"

CUICameraScript::CUICameraScript()
	: CGameCameraScript((int)SCRIPT_TYPE::UICAMERASCRIPT)
{
	m_eType = CAMERA_TYPE::UI;
}

CUICameraScript::CUICameraScript(const CUICameraScript& _origin)
	: CGameCameraScript(_origin)
{
	m_eType = CAMERA_TYPE::UI;
}

CUICameraScript::~CUICameraScript()
{
}


void CUICameraScript::begin()
{
	CGameCameraScript::begin();
}

void CUICameraScript::tick()
{
}

void CUICameraScript::SaveToFile(FILE* _pFile)
{
	CGameCameraScript::SaveToFile(_pFile);
}

void CUICameraScript::LoadFromFile(FILE* _pFile)
{
	CGameCameraScript::LoadFromFile(_pFile);
}


