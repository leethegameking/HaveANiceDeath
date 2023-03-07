#include "pch.h"
#include "CCameraPoint.h"

#include "CCameraMgr.h"

CCameraPoint::CCameraPoint()
	: CScript((int)SCRIPT_TYPE::CAMERAPOINT)
	, m_ePointType()
{
	AddScriptParam(SCRIPT_PARAM::INT, "Point Type", &m_ePointType);
}

CCameraPoint::CCameraPoint(const CCameraPoint& _origin)
	: CScript(_origin)
	, m_ePointType(_origin.m_ePointType)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Point Type", &m_ePointType);
}

CCameraPoint::~CCameraPoint()
{
}

void CCameraPoint::begin()
{
	CCameraMgr::GetInst()->RegisterCameraPoint(GetOwner(), CAMERA_POINT_TYPE(m_ePointType));
}

void CCameraPoint::tick()
{
}

void CCameraPoint::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_ePointType, sizeof(int), 1, _pFile);
}

void CCameraPoint::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_ePointType, sizeof(int), 1, _pFile);
}




