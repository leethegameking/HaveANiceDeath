#include "pch.h"
#include "ParamUI.h"

#include "CImGuiMgr.h"
#include "TreeUI.h"
#include "ListUI.h"
#include "InspectorUI.h"
#include "MaterialUI.h"
#include <Engine/CResMgr.h>

UINT ParamUI::ParamCount = 0;

void ParamUI::Param_Int(const string& _ParamName, int* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputInt(("##" + _ParamName).c_str(), _pInOut);
}

void ParamUI::Param_Float(const string& _ParamName, float* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat(("##"+_ParamName).c_str(), _pInOut);
}

void ParamUI::Param_Vec2(const string& _ParamName, Vec2* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat2(("##" + _ParamName).c_str(), *_pInOut);
}

void ParamUI::Param_Vec3(const string& _ParamName, Vec3* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat3(("##" + _ParamName).c_str(), * _pInOut);
}

void ParamUI::Param_Vec4(const string& _ParamName, Vec4* _pInOut)
{
	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat4(("##" + _ParamName).c_str(), *_pInOut);
}

void ParamUI::Param_Mat(const string& _ParamName, Matrix* _pInOut)
{
}

bool ParamUI::Param_Tex(const string& _ParamName, Ptr<CTexture>& _Tex, UI* _Inst, FUNC_1 _Func)
{
	ImGui::Text(_ParamName.c_str());

	if (nullptr != _Tex)
	{
		ImGui::Image(_Tex->GetSRV().Get(), ImVec2(100.f, 100.f));
	}
	else
	{
		ImGui::Image(nullptr, ImVec2(100.f, 100.f));
	}
	
	// �̹��� ��� �޴� �� üũ
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##ContentTree"))
		{
			// void* �� typecast
			TreeNode* pNode = (TreeNode*)payload->Data;
			CRes* pRes = (CRes*)pNode->GetData();

			if (RES_TYPE::TEXTURE == pRes->GetResType())
			{
				_Tex = (CTexture*)pRes;
			}
		}
		ImGui::EndDragDropTarget();
	}

	char szName[50] = "";
	sprintf_s(szName, 50, "##TexBtn%d", ParamCount++);

	// ��ư -> ListUI ȣ��� (Res ����Ŭ���� �Լ������ͷ� �Լ� ȣ��)
	ImGui::SameLine();
	if (ImGui::Button(szName, Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);


		// �ؽ��� ��� �޾ƿͼ� 
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(_Inst, _Func);

		pListUI->Open();

		return true;
	}

	return false;
}

void ParamUI::ShowShaderParam(CMaterial* _pMtrl)
{
	const vector<tScalarParam> vecScalar = _pMtrl->GetShader()->GetScalarParam();
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
			_pMtrl->GetScalarParam(vecScalar[i].eParam, &iData);
			ParamUI::Param_Int(vecScalar[i].strName, &iData);
			_pMtrl->SetScalarParam(vecScalar[i].eParam, &iData);
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float fData = 0;
			_pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
			ParamUI::Param_Float(vecScalar[i].strName, &fData);
			_pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
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

TEX_PARAM ParamUI::ShowTexParam(CMaterial* _pMtrl, TEX_PARAM& _eParam)
{
	TEX_PARAM eTexParam = TEX_PARAM::TEX_END;

	InspectorUI* pInspUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	 UI* pMtrlUI = pInspUI->GetResUI(RES_TYPE::MATERIAL);

	const vector<tTextureParam> vecTex = _pMtrl->GetShader()->GetTextureParam(); // ���۷����� �޾ƿ�
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = _pMtrl->GetTexParam(vecTex[i].eParam);
		// ��ư�� ���ȴ� -> ListUI Open -> ���õ� ��� TEX_PARAM���� arr�� �־���.
		if (ParamUI::Param_Tex(vecTex[i].strName, pTex, pMtrlUI, (FUNC_1)&MaterialUI::SetTexture))
		{
			// ������ �������� TEX_PARAM�� �˷���. (�� ������ �и�)
			_eParam = vecTex[i].eParam;
		}
	}

	return eTexParam;
}
