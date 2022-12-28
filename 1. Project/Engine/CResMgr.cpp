#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	: m_iLayoutOffset(0)
{
	
}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[i].begin();

		for (; iter != m_arrRes[i].end(); ++iter)
		{
			iter->second = nullptr;
		}
	}
}

Ptr<CAnimation2D> CResMgr::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm)
{
	Ptr<CAnimation2D> pAnim = FindRes<CAnimation2D>(_strKey);
	if (pAnim.Get())
		return nullptr;
	//assert(!pAnim.Get());

	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vecFrm);

	wstring strRelativeKey = L"animation\\" + _strKey + L".anim";

	pAnim->SetKey(strRelativeKey);
	pAnim->SetName(_strKey);
	AddRes<CAnimation2D>(strRelativeKey, pAnim.Get());

	pAnim->Save(strRelativeKey);

	return pAnim;
}

Ptr<CAnimation2D> CResMgr::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize, bool _bVTHZ)
{
	Ptr<CAnimation2D> pAnim = FindRes<CAnimation2D>(_strKey);
	assert(!pAnim.Get());

	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vLeftTop, _vOffset, _vSlice, _fStep, _iMaxFrm, _FPS, _vFullsize, _bVTHZ);

	wstring strRelativeKey = L"animation\\" + _strKey + L".anim";

	pAnim->SetKey(strRelativeKey);
	pAnim->SetName(_strKey);
	AddRes<CAnimation2D>(_strKey, pAnim.Get());

	pAnim->Save(strRelativeKey);

	return pAnim;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());
	
	pTex = new CTexture;
	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	pTex->SetEngineRes();
	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}



Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture;
	pTex->Create(_Tex2D);

	pTex->SetEngineRes();
	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}
