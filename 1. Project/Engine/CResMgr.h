#pragma once


#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CAnimation2D.h"

#include "CEventMgr.h"


class CResMgr
	: public CSingleton<CResMgr>
{	
private:
	map<wstring, Ptr<CRes>>				m_arrRes[(UINT)RES_TYPE::END];	
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayoutInfo;
	UINT								m_iLayoutOffset;

	// EventMgr�� ����
	// bool								m_bChanged;

public:

public:	
	template<typename T>
	void AddRes(const wstring& _strKey, T* _pRes);

	template<typename T>
	Ptr<T> FindRes(const wstring& _strKey);

	template<typename T>
	Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);


	Ptr<CTexture> CreateTexture(const wstring& _strKey
		, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag );
	Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

	const map<wstring, Ptr<CRes>>& GetResource(RES_TYPE _eResType)
	{
		return m_arrRes[(UINT)_eResType];
	}

public:
	void init();


	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutInfo() { return m_vecLayoutInfo; }

private:
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();
	void CreateDefaultMaterial();
	void CreateDefaultPrefab();
	void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName);


public:
	CResMgr();
	~CResMgr();
};

template<typename T>
RES_TYPE GetType()
{
	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		return RES_TYPE::MESH;

	if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		return RES_TYPE::TEXTURE;

	if (typeid(T).hash_code() == typeid(CGraphicsShader).hash_code())
		return RES_TYPE::GRAPHICS_SHADER;

	if (typeid(T).hash_code() == typeid(CComputeShader).hash_code())
		return RES_TYPE::COMPUTE_SHADER;

	if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		return RES_TYPE::MATERIAL;

	if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		return RES_TYPE::PREFAB;

	if (typeid(T).hash_code() == typeid(CAnimation2D).hash_code())
		return RES_TYPE::ANIMATION2D;

	else
		return RES_TYPE::END;
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, T* _pRes)
{
	RES_TYPE eResType = GetType<T>();

	CRes* pRes = FindRes<T>(_strKey).Get();

	assert(!pRes);

	_pRes->SetKey(_strKey);
	m_arrRes[(UINT)eResType].insert(make_pair(_strKey, _pRes));
	CEventMgr::GetInst()->ResChangeFlagOn();
}

template<typename T>
Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
	RES_TYPE eResType = ::GetType<T>();

	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)eResType].find(_strKey);

	if (iter == m_arrRes[(UINT)eResType].end())
	{
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template<typename T>
Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
	RES_TYPE eResType = GetType<T>();

	CRes* pResource = FindRes<T>(_strKey).Get();

	if (nullptr != pResource)
		return (T*)pResource;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pResource = new T;	
	if (FAILED(pResource->Load(strFilePath)))
	{
		MessageBox(nullptr, strFilePath.c_str(), L"���ҽ� �ε� ����", MB_OK);
		return nullptr;
	}

	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(_strKey, (T*)pResource);

	return (T*)pResource;
	CEventMgr::GetInst()->ResChangeFlagOn();
}
