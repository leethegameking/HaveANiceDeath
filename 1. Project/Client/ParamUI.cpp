#include "pch.h"
#include "ParamUI.h"

void ParamUI::Param_Int(const string& _ParamName, int* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputInt("##InputInt", _pInOut);
}

void ParamUI::Param_Float(const string& _ParamName, float* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat("##InputInt", _pInOut);
}

void ParamUI::Param_Vec2(const string& _ParamName, Vec2* _pInOut)
{
}

void ParamUI::Param_Vec4(const string& _ParamName, Vec4* _pInOut)
{
}

void ParamUI::Param_Mat(const string& _ParamName, Matrix* _pInOut)
{
}
