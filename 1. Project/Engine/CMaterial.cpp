#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CGraphicsShader.h"


CMaterial::CMaterial()
	: CRes(RES_TYPE::MATERIAL)
	, m_tConst{}
	, m_arrTex{}
{
	
}

CMaterial::CMaterial(const CMaterial& _other)
	: CRes(_other)
	, m_tConst(_other.m_tConst)
	, m_pShader(_other.m_pShader)
	, m_arrTex{}
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		m_arrTex[i] = _other.m_arrTex[i];
	}	

	m_pMasterMtrl = const_cast<CMaterial*>(&_other);
}

CMaterial::~CMaterial()
{

}

void CMaterial::UpdateData()
{
	// 텍스쳐 업데이트
	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			m_arrTex[i]->UpdateData(i, ALL_STAGE);
			m_tConst.iTexCheck[i] = true;
		}		
		else
		{
			m_tConst.iTexCheck[i] = false;
		}
	}

	// 상수 업데이트
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_tConst);
	pCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);

	// 쉐이더 업데이트
	m_pShader->UpdateData();
}

void CMaterial::Clear()
{
	for (UINT i = 0; i < TEX_END; ++i)
	{
		CTexture::Clear(i);
	}	
}

void CMaterial::SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData)
{
	switch (_eScalarType)
	{
	case INT_0:		
	case INT_1:		
	case INT_2:		
	case INT_3:
		m_tConst.iArr[_eScalarType] = *((int*)_pData);
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_tConst.fArr[_eScalarType - FLOAT_0] = *((float*)_pData);
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_tConst.v2Arr[_eScalarType - VEC2_0] = *((Vec2*)_pData);
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_tConst.v4Arr[_eScalarType - VEC4_0] = *((Vec4*)_pData);
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_tConst.matArr[_eScalarType - MAT_0] = *((Matrix*)_pData);
		break;	
	}
}

void CMaterial::GetScalarParam(SCALAR_PARAM _eScalarType, void* _pData)
{
	switch (_eScalarType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		*((int*)_pData) = m_tConst.iArr[_eScalarType];
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		*((float*)_pData) = m_tConst.fArr[_eScalarType - FLOAT_0];
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		*((Vec2*)_pData) = m_tConst.v2Arr[_eScalarType - VEC2_0];
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		*((Vec4*)_pData) = m_tConst.v4Arr[_eScalarType - VEC4_0];
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		*((Matrix*)_pData) = m_tConst.matArr[_eScalarType - MAT_0];
		break;
	}

}

void CMaterial::SetTexParam(TEX_PARAM _eTex, Ptr<CTexture> _pTex)
{
	m_arrTex[_eTex] = _pTex;
}

Ptr<CTexture> CMaterial::GetTexParam(TEX_PARAM _eTex)
{
	return m_arrTex[(UINT)_eTex];
}


void CMaterial::Save(const wstring& _strRelativePath)
{
	if (!CheckRelativePath(_strRelativePath))
		assert(nullptr);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// CRes의 Key Path 저장
	SaveKeyPath(pFile);

	//  셰이더
	SaveResourceRef(m_pShader, pFile);

	// texture, 상수버퍼
	if (nullptr != m_pShader)
	{
		fwrite(&m_tConst, sizeof(tMtrlConst), 1, pFile);

		for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
		{
			SaveResourceRef(m_arrTex[i], pFile);
		}
	}

	fclose(pFile);
}

int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	if (FAILED(_wfopen_s(&pFile, _strFilePath.c_str(), L"rb")))
		return E_FAIL;

	LoadKeyPath(pFile);

	LoadResourceRef(m_pShader, pFile);

	if (nullptr != m_pShader)
	{
		fread(&m_tConst, sizeof(tMtrlConst), 1, pFile);

		for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
		{
			LoadResourceRef(m_arrTex[i], pFile);
		}
	}

	fclose(pFile);

	return S_OK;
}

void CMaterial::DynamicSave(FILE* _File)
{
	//  셰이더
	SaveResourceRef(m_pShader, _File);

	// texture, 상수버퍼
	if (nullptr != m_pShader)
	{
		fwrite(&m_tConst, sizeof(tMtrlConst), 1, _File);

		for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
		{
			SaveResourceRef(m_arrTex[i], _File);
		}
	}
}

void CMaterial::DynamicLoad(FILE* _File)
{
	LoadResourceRef(m_pShader, _File);

	if (nullptr != m_pShader)
	{
		fread(&m_tConst, sizeof(tMtrlConst), 1, _File);

		for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
		{
			LoadResourceRef(m_arrTex[i], _File);
		}
	}
}
