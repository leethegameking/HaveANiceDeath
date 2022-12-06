#pragma once
class ParamUI
{
public:
	static void Param_Int(const string& _ParamName, int* _pInOut);
	static void Param_Float(const string& _ParamName, float* _pInOut);
	static void Param_Vec2(const string& _ParamName, Vec2* _pInOut);
	static void Param_Vec4(const string& _ParamName, Vec4* _pInOut);
	static void Param_Mat(const string& _ParamName, Matrix* _pInOut);
};

