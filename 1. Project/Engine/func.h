#pragma once


template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	typename vector<T*>::iterator iter = _vec.begin();
	for (; iter != _vec.end(); ++iter)
	{
		if (nullptr != (*iter))
			delete (*iter);
	}
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}

template<typename T, int SIZE>
void Safe_Del_Array(T* (&_arr)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}

Vec4 MulMatrix(Vec4 _Vec4 ,const Matrix& _mat);

int GetSizeofFormat(DXGI_FORMAT _eFormat);

class CGameObject;
bool IsValid(CGameObject*& _pObj);

class CEntity;
inline bool IsValid(CEntity* _pCom)
{
	if (nullptr == _pCom)
		return false;
	return true;
}

const char* ToString(COMPONENT_TYPE _type);
const wchar_t* ToWString(COMPONENT_TYPE _type);
const char* ToString(RES_TYPE _type);
const wchar_t* ToWString(RES_TYPE _type);

// ===============
// Event ���� �Լ�
// ===============
class CGameObject;
void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx = 0);


// ==========
// Debug Draw
// ==========
#ifdef _DEBUG
void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration = 0.f);
void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration = 0.f);

//void DebugDrawCube();
//void DebugDrawSphere();
#endif