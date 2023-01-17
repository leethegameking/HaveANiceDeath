#pragma once
class CCameraMgr
	: public CSingleton<CCameraMgr>
{
	SINGLETON(CCameraMgr)

private:
	CGameObject* m_arrCameraObj[(int)CAMERA_TYPE::END];

public:
	void RegisterCamera(CGameObject* _pObj);
	void Clear();

	CGameObject* GetUICamera();
	CGameObject* GetMainCamera();
	CGameObject* GetCutSceneCamera();
	CGameObject* GetBossCamera();
};

