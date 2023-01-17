#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>

#include "CImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"
#include "ParamUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", COMPONENT_TYPE::MESHRENDER)
	, m_eSelectTexParam(TEX_PARAM::TEX_END)
	, m_bDynamicMtrl(false)
{
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::update()
{


	ComponentUI::update();
}

void MeshRenderUI::render_update()
{
	if (GetTarget())
	{
		m_Mesh = GetTarget()->MeshRender()->GetMesh();
		m_Mtrl = GetTarget()->MeshRender()->GetCurMaterial();
		if (GetTarget()->MeshRender()->IsDynamicMtrl())
		{
			m_bDynamicMtrl = true;
		}
		else
		{
			m_bDynamicMtrl = false;
		}
	}

	// 컴포넌트 타입 텍스트 버튼
	ComponentUI::render_update();
	if (!IsOpen())
		return;

	string MeshName, MtrlName;

	if (m_Mesh.Get())
	{
		MeshName = string(m_Mesh->GetKey().begin(), m_Mesh->GetKey().end());
	}

	if (m_Mtrl.Get())
	{
		MtrlName = string(m_Mtrl->GetKey().begin(), m_Mtrl->GetKey().end());
	}

	ImGui::Text("Mesh     ");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)MeshName.data(), MeshName.length(), ImGuiInputTextFlags_ReadOnly);



	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUIModal"));
		assert(pListUI);

		// 메쉬 목록을 받아와서 ListUI에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MESH);
		static vector<wstring> vecRes;
		vecRes.clear();

		// 맵에서 이름만 뽑아서 벡터에 전달해줌.
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		// 이름 ItemList로 복사
		pListUI->init(WstrToStrVec(vecRes));

		// Item이 DoubleClick되었을때 이 객체의 SetMesh함수를 호출한다. 
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MeshRenderUI::SetMesh);

		pListUI->Open();
	}

	ImGui::Text("Material ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)MtrlName.data(), MtrlName.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##ContentTree"))
		{
			TreeNode* pNode = (TreeNode*)payload->Data;
			CRes* pRes = (CRes*)pNode->GetData();

			if (RES_TYPE::MATERIAL == pRes->GetResType())
			{
				GetTarget()->MeshRender()->SetSharedMaterial((CMaterial*)pRes);
			}
		}


		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	MtrlBtn();

	
	if (ImGui::RadioButton("Shared", !m_bDynamicMtrl))
	{
		GetTarget()->MeshRender()->GetSharedMaterial();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Dynamic", m_bDynamicMtrl))
	{
		GetTarget()->MeshRender()->GetDynamicMaterial();
	}

	if (m_bDynamicMtrl)
	{
		ParamUI::ShowShaderParam(m_Mtrl.Get());

		// Tex Select
		const vector<tTextureParam> vecTex = m_Mtrl->GetShader()->GetTextureParam();
		for (size_t i = 0; i < vecTex.size(); ++i)
		{
			Ptr<CTexture> pTex = m_Mtrl->GetTexParam(vecTex[i].eParam);
			if (ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MeshRenderUI::SetTexture))
			{
				m_eSelectTexParam = vecTex[i].eParam;
			}
			else
			{
				m_Mtrl->SetTexParam(vecTex[i].eParam, pTex);
			}
		}
	}

	// Dynamic Texture 강제 설정
	/*ImGui::Text("Mtrl Tex ");
	ImGui::SameLine();

	if (m_bDynamicMtrl)
	{
		Ptr<CTexture> pDynTex = m_Mtrl->GetTexArr()[0];
		if (pDynTex.Get())
		{
			ImGui::InputText("##TexName", (char*)WstrToStr(pDynTex.Get()->GetName()).c_str(), MtrlName.length(), ImGuiInputTextFlags_ReadOnly);
		}
		ImGui::SameLine();
	}
	TextureBtn();*/
}

void MeshRenderUI::MtrlBtn()
{
	if (ImGui::Button("##MtrlBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUIModal"));
		assert(pListUI);

		// 재질 목록을 받아와서 ListUI에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
		static vector<wstring> vecRes;
		vecRes.clear();

		// 맵에서 이름만 뽑아서 벡터에 전달해줌.
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		// 이름 ItemList로 복사
		pListUI->SetItemList(vecRes);

		// Item이 DoubleClick되었을때 이 객체의 SetMesh함수를 호출한다. 
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MeshRenderUI::SetMaterial);

		pListUI->Open();
	}
}

void MeshRenderUI::TextureBtn()
{
	if (ImGui::Button("##MtrlTextureBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUIModal"));
		assert(pListUI);

		// 재질 목록을 받아와서 ListUI에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		// 맵에서 이름만 뽑아서 벡터에 전달해줌.
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		// 이름 ItemList로 복사
		pListUI->SetItemList(vecRes);

		// Item이 DoubleClick되었을때 이 객체의 SetMesh함수를 호출한다. 
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MeshRenderUI::SetDynMaterialTex);

		pListUI->Open();
	}
}

void MeshRenderUI::SetMesh(DWORD_PTR _strMeshKey)
{
	string strKey = (char*)_strMeshKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstrKey);
	assert(nullptr != pMesh);

	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SetMaterial(DWORD_PTR _strMaterialKey)
{
	string strKey = (char*)_strMaterialKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstrKey);
	assert(nullptr != pMtrl);

	GetTarget()->MeshRender()->SetSharedMaterial(pMtrl);
}

void MeshRenderUI::SetDynMaterialTex(DWORD_PTR _strTextureKey)
{
	string strKey = (char*)_strTextureKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(nullptr != pTex);

	Ptr<CMaterial> pDynMtrl = GetTarget()->MeshRender()->GetDynamicMaterial();
	pDynMtrl->SetTexParam(TEX_PARAM::TEX_0, pTex);
}

void MeshRenderUI::Close()
{
	UI::Close();

}

void MeshRenderUI::SetTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = StrToWstr(strKey);

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(pTex.Get());

	m_Mtrl->SetTexParam(m_eSelectTexParam, pTex);
}




