#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CMaterial.h>
#include "ParamUI.h"

MaterialUI::MaterialUI()
	: ResUI("##MaterialUI", RES_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::update()
{
	ResUI::update();
}

void MaterialUI::render_update()
{
	ResUI::render_update();

	CMaterial* pMtrl = (CMaterial*)GetTarget().Get();

	string strKey = WstrToStr(pMtrl->GetKey());
	string strRelativePath = WstrToStr(pMtrl->GetRelativePath());

	string strShaderKey;
	if (nullptr != pMtrl->GetShader())
	{
		strShaderKey = WstrToStr(pMtrl->GetShader()->GetKey());
	}


	ImGui::Text("Key");
	ImGui::SameLine();
	ImGui::InputText("##MtrlKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Path          ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlPath", (char*)strRelativePath.data(), strRelativePath.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("GraphicsShader");
	ImGui::SameLine();
	ImGui::InputText("##ShaderName", (char*)strShaderKey.data(), strShaderKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::Text("Shader Parameter");

	const vector<tScalarParam> vecScalar = pMtrl->GetShader()->GetScalarParam();
	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int iData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &iData);
			ParamUI::Param_Int(vecScalar[i].strName, &iData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &iData);
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float fData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
			ParamUI::Param_Float(vecScalar[i].strName, &fData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{

		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{

		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{

		}
		break;
		}
	}
}


