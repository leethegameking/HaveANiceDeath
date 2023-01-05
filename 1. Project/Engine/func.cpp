#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath)
{
	wstring strRelativePath;
	if (-1 == _strPath.find(_strBase))
	{
		return strRelativePath;
	}

	strRelativePath = _strPath.substr(_strBase.length(), _strPath.length());
	return strRelativePath;
}

wstring GetNameFromPath(const wstring& _strPath)
{
	size_t StartIdx = _strPath.rfind(L"\\");
	size_t EndIdx = _strPath.rfind(L".");
	wstring strName = _strPath.substr(StartIdx + 1, EndIdx - StartIdx - 1);

	return strName;
}

bool CalBit(UINT _originBit, UINT _cmpBits, BIT_FUNC_OPT _opt)
{
	switch (_opt)
	{
	case BIT_LEAST_ONE:
	{
		if (_originBit & _cmpBits)
			return true;
		return false;
	}
		break;

	case BIT_EQUAL:
	{
		if (_originBit - _cmpBits == 0)
			return true;
		return false;
	}
		break;

	case BIT_INCLUDE:
	{
		if ((_originBit & _cmpBits) == _cmpBits)
			return true;
		return false;
	}
		break;


	}
}

void AddBit(UINT& _src, UINT _add)
{
	_src |= _add;
}

void RemoveBit(UINT& _src, UINT _remove)
{
	_src &= ~_remove;
}

bool InCamera(CGameObject* _obj, Vec2 _cameraExtenseion)
{
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	float vCamScale = mainCam->GetOrthographicScale();
	static Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	Vec2 vScreenCoord = vCamScale * vRenderResol;

	Vec3 vCamPos = mainCam->Transform()->GetWorldPos();
	Vec3 vPos =_obj->Transform()->GetWorldPos();
	Vec3 vScale = _obj->Transform()->GetWorldScale();

	if (
		vCamPos.x - vScreenCoord.x * _cameraExtenseion.x / 2.f < vPos.x + vScale.x / 2.f &&
		vCamPos.x + vScreenCoord.x * _cameraExtenseion.x / 2.f > vPos.x - vScale.x / 2.f &&
		vCamPos.y - vScreenCoord.y * _cameraExtenseion.y / 2.f < vPos.y + vScale.y / 2.f &&
		vCamPos.y + vScreenCoord.y * _cameraExtenseion.y / 2.f > vPos.y - vScale.y / 2.f)
	{
		return true;
	}
	return false;
}


Vec4 MulMatrix(Vec4 _Vec4, const Matrix& _mat)
{
	return
	Vec4(_Vec4.x* _mat._11 + _Vec4.y * _mat._21 + _Vec4.z * _mat._31 + _Vec4.w * _mat._41,
		 _Vec4.x* _mat._12 + _Vec4.y * _mat._22 + _Vec4.z * _mat._32 + _Vec4.w * _mat._42,
		 _Vec4.x* _mat._13 + _Vec4.y * _mat._23 + _Vec4.z * _mat._33 + _Vec4.w * _mat._43,
		 _Vec4.x* _mat._14 + _Vec4.y * _mat._24 + _Vec4.z * _mat._34 + _Vec4.w * _mat._44);

}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

#include "CGameObject.h"
bool IsValid(CGameObject*& _pObj)
{
	if (nullptr == _pObj)
		return false;

	if (_pObj->IsDead())
	{
		_pObj = nullptr;
		return false;
	}

	return true;
}


#include "CGameObject.h"
#include "CTransform.h"
#include "CEventMgr.h"

void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx)
{
	_pNewObj->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};
	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

#include "CRenderMgr.h"

void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuratiion)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::RECT, _vColor, _vPosition, _vScale, _vRotation, 0.f, _fDuratiion);
}

void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::CIRCLE, _vColor, _vPosition, Vec3::Zero, Vec3::Zero, _fRadius, _fDuration);
}

void SaveStringToFile(const string& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.data(), sizeof(char), iLen, _pFile);
}

void LoadStringFromFile(string& _str, FILE* _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	char szBuffer[256] = {};
	fread(szBuffer, sizeof(char), iLen, _pFile); // 바로 받을 수 있나? void* 로
	_str = szBuffer;
}

void SaveWStringToFile(const wstring& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.data(), sizeof(wchar_t), iLen, _pFile);
}

void LoadWStringFromFile(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	wchar_t szBuffer[256] = {};
	fread(szBuffer, sizeof(wchar_t), iLen, _pFile);
	_str = szBuffer;
}

vector<wstring> StrToWStrVec(const vector<string>& _vecStr)
{
	vector<wstring> vecWstr;

	for (size_t i = 0; i < _vecStr.size(); ++i)
	{
		vecWstr.push_back(StrToWstr(_vecStr[i]));
	}
	return vecWstr;
}

vector<string> WstrToStrVec(const vector<wstring>& _vecWstr)
{
	vector<string> vecStr;

	for (size_t i = 0; i <_vecWstr.size(); ++i)
	{
		vecStr.push_back(WstrToStr(_vecWstr[i]));
	}
	return vecStr;
}

const char* ToString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_WCHAR[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
	return RES_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(RES_TYPE _type)
{
	return RES_TYPE_WCHAR[(UINT)_type];
}