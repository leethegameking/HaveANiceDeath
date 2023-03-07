#pragma once
class CEffectMgr
	: public CSingleton<CEffectMgr>
{
	SINGLETON(CEffectMgr)
private:
	Ptr<CRes> m_arrFXPref[(int)FX_TYPE::END];
	CGameObject* m_arrFXObj[(int)FX_TYPE::END];

public:
	void init();

	void RegisterFX(Ptr<CRes> _pPref, FX_TYPE _eType);
	CGameObject* GetFXObj(FX_TYPE _eType);

	void DestroyFX(FX_TYPE _eType);
};

