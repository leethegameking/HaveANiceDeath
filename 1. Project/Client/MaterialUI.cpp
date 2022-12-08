#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMaterial.h>
#include "ParamUI.h"



MaterialUI::MaterialUI()
	: ResUI("##MaterialUI", RES_TYPE::MATERIAL)
	, m_eSelectTexParam(TEX_PARAM::TEX_END)
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

	const vector<tTextureParam> vecTex = pMtrl->GetShader()->GetTextureParam(); // 레퍼런스로 받아옴
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);
		// 버튼이 눌렸다 -> ListUI Open -> 선택된 멤버 TEX_PARAM으로 arr에 넣어줌.
		if(ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MaterialUI::SetTexture))
		{
			// 선택한 아이템의 TEX_PARAM을 알려줌. (한 프레임 밀림)
			m_eSelectTexParam = vecTex[i].eParam;
		}
		// 버튼이 안 눌렸다.
		else
		{
			// 굳이?
			pMtrl->SetTexParam(vecTex[i].eParam, pTex);
		}
		
	}
}


void MaterialUI::SetTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = StrToWstr(strKey);

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(pTex.Get());

	CMaterial* pMtrl = (CMaterial*)GetTarget().Get();
	pMtrl->SetTexParam(m_eSelectTexParam, pTex);
}