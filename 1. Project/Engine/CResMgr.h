#pragma once


#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CAnimation2D.h"
#include "CSound.h"

#include "CEventMgr.h"


class CResMgr
	: public CSingleton<CResMgr>
{	
private:
	map<wstring, Ptr<CRes>>				m_arrRes[(UINT)RES_TYPE::END];	
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayoutInfo;
	UINT								m_iLayoutOffset;

	// EventMgr로 위임
	// bool								m_bChanged;

public:

public:	
	template<typename T>
	void AddRes(const wstring& _strKey, T* _pRes);

	template<typename T>
	Ptr<T> FindRes(const wstring& _strKey);

	template<typename T>
	Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

	template<typename T>
	Ptr<T> Load(const wstring& _strRelativePath);

	template<typename T>
	wstring	GetNewResName(const wstring& _name = L"");

public:
	Ptr<CAnimation2D> CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm);
	Ptr<CAnimation2D> CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize = Vec2(400.f, 400.f), bool _bVTHZ = HORIZONTAL);
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
	bool DeleteRes(RES_TYPE _Type, const wstring& _strKey);

private:
	void InitSound();
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();
	void CreateDefaultMaterial();
	void CreateDefaultAninmation();
	void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName);

private:
	void CreateMyGraphicsShader();
	void CreateMyCSShader();

private:
	void CreateBlockInnerShader();
	void CreateBlockOuterShader();

public:
	CResMgr();
	~CResMgr();

	friend class CEventMgr;
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

	if (typeid(T).hash_code() == typeid(CSound).hash_code())
		return RES_TYPE::SOUND;

	else
		return RES_TYPE::END;
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, T* _pRes)
{
	RES_TYPE eResType = GetType<T>();

	CRes* pRes = FindRes<T>(_strKey).Get();

	// assert(!pRes);
	if(pRes)
		return;

	_pRes->SetKey(_strKey);
	// 엔진 리소스면 키 값으로 이름 설정
	if (_pRes->IsEngineRes())
	{
		_pRes->SetName(_strKey);
	}
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
		MessageBox(nullptr, strFilePath.c_str(), L"리소스 로딩 실패", MB_OK);
		return nullptr;
	}

	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(_strKey, (T*)pResource);

	CEventMgr::GetInst()->ResChangeFlagOn();
	return (T*)pResource;
	
}

template<typename T>
Ptr<T> CResMgr::Load(const wstring& _strRelativePath)
{
	RES_TYPE eResType = GetType<T>();

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	CRes* pResource = new T;
	if (FAILED(pResource->Load(strFilePath)))
	{
		MessageBox(nullptr, strFilePath.c_str(), L"리소스 로딩 실패", MB_OK);
		return nullptr;
	}

	wstring wstrKey = pResource->GetKey();

	CRes* pCheckRes = FindRes<T>(wstrKey).Get();

	assert(!pCheckRes);
	
	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(wstrKey, (T*)pResource);

	CEventMgr::GetInst()->ResChangeFlagOn();
	return (T*)pResource;
}

template<typename T>
inline wstring CResMgr::GetNewResName(const wstring& _name)
{
	RES_TYPE type = GetType<T>();

	wstring strName = L"New ";
	wstring strFolderName = L"";
	wstring strExt = L"";

	switch (type)
	{
	case RES_TYPE::PREFAB:
		strName = _name;
		strFolderName = L"prefab\\";
		strExt = L".pref";
		break;
	case RES_TYPE::MESHDATA:
		break;
	case RES_TYPE::COMPUTE_SHADER:
		break;
	case RES_TYPE::MATERIAL:
		strName += L"Material";
		strFolderName = L"material\\";
		strExt = L".mtrl";
		break;
	case RES_TYPE::MESH:
		break;
	case RES_TYPE::TEXTURE:
		break;
	case RES_TYPE::SOUND:
		break;
	case RES_TYPE::GRAPHICS_SHADER:
		break;
	case RES_TYPE::ANIMATION2D:
		break;
	case RES_TYPE::END:
		break;
	}


	// 이름 중복 막기
	UINT Count = 0;
	while (true)
	{
		string strCount = "(" +  to_string(Count) + ")";
		wstring wstrCount = StrToWstr(strCount);

		wstring strTemp = strName;
		if (Count == 0)
		{
			strTemp = strFolderName + strTemp + strExt;
		}
		else
		{
			strTemp = strFolderName + strTemp + wstrCount + strExt;
		}

		if (nullptr != FindRes<T>(strTemp))
		{
			++Count;
		}
		else
		{
			if (!Count == 0)
			{
				strName += wstrCount;
			}
			break;
		}
	}

	return strName;
}