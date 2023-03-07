#include "pch.h"
#include "CCameraMgr.h"

#include <Engine/CGameObject.h>
#include <Engine/GlobalComponent.h>
#include "CGameCameraScript.h"

CCameraMgr::CCameraMgr()
	: m_arrCameraObj{}
{

}

CCameraMgr::~CCameraMgr()
{

}

void CCameraMgr::RegisterCamera(CGameObject* _pObj)
{
	for (int i = 0; i < (int)CAMERA_TYPE::END; ++i)
	{
		if (m_arrCameraObj[i] == _pObj)
			return;
	}

	CAMERA_TYPE eType = _pObj->GetScript<CGameCameraScript>()->GetType();
	m_arrCameraObj[(int)eType] = _pObj;
}

void CCameraMgr::RegisterCameraPoint(CGameObject* _pObj, CAMERA_POINT_TYPE _eType)
{
	m_arrCamPoint[(int)_eType] = _pObj;
}

void CCameraMgr::Clear()
{
}

CGameObject* CCameraMgr::GetUICamera()
{
	return m_arrCameraObj[(int)CAMERA_TYPE::UI];
}

CGameObject* CCameraMgr::GetMainCamera()
{
	return m_arrCameraObj[(int)CAMERA_TYPE::MAIN];
}

CGameObject* CCameraMgr::GetCutSceneCamera()
{
	return m_arrCameraObj[(int)CAMERA_TYPE::CUT_SCENE];
}

CGameObject* CCameraMgr::GetBossCamera()
{
	return m_arrCameraObj[(int)CAMERA_TYPE::BOSS];
}

CGameObject* CCameraMgr::GetCamPoint(CAMERA_POINT_TYPE _eType)
{
	return  m_arrCamPoint[(int)_eType];
}
