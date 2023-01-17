#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMaterial.h>

#include "CImGuiMgr.h"
#include "ParamUI.h"
#include "CommonUI.h"
#include "ListUI.h"



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

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUIModal"));
		assert(pListUI);

		// �޽� ����� �޾ƿͼ� ListUI�� ����
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::GRAPHICS_SHADER);
		static vector<wstring> vecRes;
		vecRes.clear();

		// �ʿ��� �̸��� �̾Ƽ� ���Ϳ� ��������.
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		// �̸� ItemList�� ����
		pListUI->init(WstrToStrVec(vecRes));

		// Item�� DoubleClick�Ǿ����� �� ��ü�� SetMesh�Լ��� ȣ���Ѵ�. 
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MaterialUI::SetGraphicsShader);

		pListUI->Open();
	}

	if (CommonUI::ButtonCenteredOnLine("Save"))
	{
		Ptr<CMaterial> ptrMtrl = (CMaterial*)GetTarget().Get();
		ptrMtrl->Save(ptrMtrl->GetKey());
		CommonUI::OpenPopup("Material Save!");
	}
	CommonUI::NotifyPopup();


	if (pMtrl->GetShader().Get())
	{
		ImGui::NewLine();
		ImGui::Text("Shader Parameter");
		ParamUI::ShowShaderParam(pMtrl);
		//ParamUI::ShowTexParam(pMtrl , m_eSelectTexParam);
		const vector<tTextureParam> vecTex = pMtrl->GetShader()->GetTextureParam();
		for (size_t i = 0; i < vecTex.size(); ++i)
		{
			Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);
			if (ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MaterialUI::SetTexture))
			{
				m_eSelectTexParam = vecTex[i].eParam;
			}
			else
			{
				pMtrl->SetTexParam(vecTex[i].eParam, pTex);
			}
		}
	}


}

void MaterialUI::SetGraphicsShader(DWORD_PTR _ShaderKey)
{
	wstring wstrKey = StrToWstr((char*)_ShaderKey);
	Ptr<CMaterial> ptrMtrl = (CMaterial*)GetTarget().Get();
	ptrMtrl->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(wstrKey));
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

//void MaterialUI::ShowShaderParam(CMaterial* _pMtrl)
//{
//	const vector<tScalarParam> vecScalar = _pMtrl->GetShader()->GetScalarParam();
//	for (size_t i = 0; i < vecScalar.size(); ++i)
//	{
//		switch (vecScalar[i].eParam)
//		{
//		case INT_0:
//		case INT_1:
//		case INT_2:
//		case INT_3:
//		{
//			int iData = 0;
//			_pMtrl->GetScalarParam(vecScalar[i].eParam, &iData);
//			ParamUI::Param_Int(vecScalar[i].strName, &iData);
//			_pMtrl->SetScalarParam(vecScalar[i].eParam, &iData);
//		}
//		break;
//		case FLOAT_0:
//		case FLOAT_1:
//		case FLOAT_2:
//		case FLOAT_3:
//		{
//			float fData = 0;
//			_pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
//			ParamUI::Param_Float(vecScalar[i].strName, &fData);
//			_pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
//		}
//		break;
//		case VEC2_0:
//		case VEC2_1:
//		case VEC2_2:
//		case VEC2_3:
//		{
//
//		}
//		break;
//		case VEC4_0:
//		case VEC4_1:
//		case VEC4_2:
//		case VEC4_3:
//		{
//
//		}
//		break;
//		case MAT_0:
//		case MAT_1:
//		case MAT_2:
//		case MAT_3:
//		{
//
//		}
//		break;
//		}
//	}
//}
//
//void MaterialUI::ShowTexParam(CMaterial* _pMtrl)
//{
//	const vector<tTextureParam> vecTex = _pMtrl->GetShader()->GetTextureParam(); // ���۷����� �޾ƿ�
//	for (size_t i = 0; i < vecTex.size(); ++i)
//	{
//		Ptr<CTexture> pTex = _pMtrl->GetTexParam(vecTex[i].eParam);
//		// ��ư�� ���ȴ� -> ListUI Open -> ���õ� ��� TEX_PARAM���� arr�� �־���.
//		if (ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MaterialUI::SetTexture))
//		{
//			// ������ �������� TEX_PARAM�� �˷���. (�� ������ �и�)
//			m_eSelectTexParam = vecTex[i].eParam;
//		}
//		// ��ư�� �� ���ȴ�.
//		else
//		{
//			// ����?
//			_pMtrl->SetTexParam(vecTex[i].eParam, pTex);
//		}
//
//	}
//}

