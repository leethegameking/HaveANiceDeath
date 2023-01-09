#pragma once

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

#include "UI.h"

class CPrefab;

class ParamUI
{
private:
	static UINT ParamCount;

public:
	static void Param_Bool(const string& _ParamName, bool* _bInOut);
	static void Param_Int(const string& _ParamName, int* _pInOut);
	static void Param_Float(const string& _ParamName, float* _pInOut);
	static void Param_Vec2(const string& _ParamName, Vec2* _pInOut);
	static void Param_Vec3(const string& _ParamName, Vec3* _pInOut);
	static void Param_Vec4(const string& _ParamName, Vec4* _pInOut);
	static void Param_Mat(const string& _ParamName, Matrix* _pInOut);

	static void Param_Prefab(const string& _ParamName, Ptr<CPrefab>& _Pref);

	static bool Param_Tex(const string& _ParamName, Ptr<CTexture>& _Tex, UI* _Inst, FUNC_1 _Func);

public:
	static void ShowShaderParam(CMaterial* _pMtrl);
	static TEX_PARAM ShowTexParam(CMaterial* _pMtrl, TEX_PARAM& _eParam);

	friend class CImGuiMgr;
};

