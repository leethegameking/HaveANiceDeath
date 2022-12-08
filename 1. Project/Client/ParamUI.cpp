#include "pch.h"
#include "ParamUI.h"

#include "CImGuiMgr.h"
#include "TreeUI.h"
#include "ListUI.h"
#include <Engine/CResMgr.h>

UINT ParamUI::ParamCount = 0;

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
	ImGui::InputFloat("##InputFloat", _pInOut);
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
