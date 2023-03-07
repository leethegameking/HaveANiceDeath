#pragma once
class CCameraMgr
	: public CSingleton<CCameraMgr>
{
	SINGLETON(CCameraMgr)

private:
	CGameObject* m_arrCameraObj[(int)CAMERA_TYPE::END];
	CGameObject* m_arrCamPoint[(int)CAMERA_POINT_TYPE::END];

public:
	void RegisterCamera(CGameObject* _pObj);
	void RegisterCameraPoint(CGameObject* _pObj, CAMERA_POINT_TYPE _eType);

	void Clear();

	CGameObject* GetUICamera();
	CGameObject* GetMainCamera();
	CGameObject* GetCutSceneCamera();
	CGameObject* GetBossCamera();

	CGameObject* GetCamPoint(CAMERA_POINT_TYPE _eType);
};

