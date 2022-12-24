#include "pch.h"
#include "CRes.h"


CRes::CRes(RES_TYPE _eResType)
	: m_eResType(_eResType)
	, m_iRefCount(0)
{
}

CRes::CRes(const CRes& _other)
	: CEntity(_other)
	, m_eResType(_other.m_eResType)
	, m_strKey(_other.m_strKey)
	, m_strRelativePath(_other.m_strRelativePath)
	, m_iRefCount(0)
	, m_bEngineRes(false)
{
}

CRes::~CRes()
{
	assert(!(0 < m_iRefCount));
}

void CRes::SaveKeyPath(FILE* _pFile)
{
	SaveWStringToFile(m_strKey, _pFile);
	SaveWStringToFile(m_strRelativePath, _pFile);
}

void CRes::LoadKeyPath(FILE* _pFile)
{
	LoadWStringFromFile(m_strKey, _pFile);
	LoadWStringFromFile(m_strRelativePath, _pFile);
}

bool CRes::CheckRelativePath(const wstring& _strRelativePath)
{
	// first save
	if (m_strRelativePath.empty())
	{
		m_strRelativePath = _strRelativePath;
		return true;
	}
	// additional save
	else
	{
		// same path 
		if (m_strRelativePath == _strRelativePath)
			return true;
		else
			return false;
	}

	return false;
}
