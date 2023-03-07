#include "pch.h"
#include "ParticleSystemUI.h"

#include "ListUI.h"
#include "CImGuiMgr.h"
#include "ComboBox.h"
#include <Engine/CResMgr.h>

#include <Engine/CParticleSystem.h>
#include <Engine/CResMgr.h>
#include "ParamUI.h"
#include "TreeUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
{
	m_CSComboBox = new ComboBox;
}

ParticleSystemUI::~ParticleSystemUI()
{
	if (m_CSComboBox)
		delete m_CSComboBox;
}

void ParticleSystemUI::SetTexture(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	GetTarget()->ParticleSystem()->SetTexture(wstrKey);
}

void ParticleSystemUI::init()
{
	wstring strCSKey = GetTarget()->ParticleSystem()->GetCSKey();

	map<wstring, Ptr<CRes>> mapCS = CResMgr::GetInst()->GetResource(RES_TYPE::COMPUTE_SHADER);
	map<wstring, Ptr<CRes>>::const_iterator iter = mapCS.begin();
	int i = 0;
	int idx = 0;
	vector<string> vecItemList;
	vecItemList.reserve(mapCS.size());
	for (; iter != mapCS.end(); ++iter)
	{
		vecItemList.push_back(WstrToStr(iter->first));
		if (iter->first == strCSKey)
		{
			idx = i;
		}
		else
		{
			++i;
		}
	}

	m_CSComboBox->init<CComputeShader>(vecItemList, idx);
	m_CSComboBox->AddSelectedFunc(this, (FUNC_1)&ParticleSystemUI::SetCSKey);
}

void ParticleSystemUI::update()
{


	ComponentUI::update();
}

void ParticleSystemUI::render_update()
{
	if (GetTarget())
	{
		m_Mesh = GetTarget()->ParticleSystem()->GetMesh();
		m_Mtrl = GetTarget()->ParticleSystem()->GetCurMaterial();
		if (GetTarget()->ParticleSystem()->IsDynamicMtrl())
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
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetMesh);

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
				GetTarget()->ParticleSystem()->SetSharedMaterial((CMaterial*)pRes);
			}
		}


		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	MtrlBtn();


	if (ImGui::RadioButton("Shared", !m_bDynamicMtrl))
	{
		GetTarget()->ParticleSystem()->GetSharedMaterial();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Dynamic", m_bDynamicMtrl))
	{
		GetTarget()->ParticleSystem()->GetDynamicMaterial();
	}

	if (GetTarget())
	{
		m_iMaxCount = GetTarget()->ParticleSystem()->GetMaxCount();
		m_iAliveCount = GetTarget()->ParticleSystem()->GetAliveCount();

		m_vStartScale = GetTarget()->ParticleSystem()->GetStartScale();
		m_vEndScale = GetTarget()->ParticleSystem()->GetEndScale();

		m_vStartColor = GetTarget()->ParticleSystem()->GetStartColor();
		m_vEndColor = GetTarget()->ParticleSystem()->GetEndColor();

		m_vMinMaxSpeed = GetTarget()->ParticleSystem()->GetMinMaxSpeed();
		m_vMinMaxLifeTime = GetTarget()->ParticleSystem()->GetMinMaxLife();

		m_fSpawnRange = GetTarget()->ParticleSystem()->GetSpawnRange();

		m_Frequency = GetTarget()->ParticleSystem()->GetFrequency();
		m_fAccTime = GetTarget()->ParticleSystem()->GetAccTime();

		m_WorldSpawn = GetTarget()->ParticleSystem()->GetWorldSpawn();


		wstring texKey = GetTarget()->ParticleSystem()->GetTexKey();
		m_TexKey = string(texKey.begin(), texKey.end());

		wstring CSKey = GetTarget()->ParticleSystem()->GetUpdateCS()->GetKey();
		m_CSKey = WstrToStr(CSKey);

		//========================================================================

		m_Mesh = GetTarget()->ParticleSystem()->GetMesh();
		m_Mtrl = GetTarget()->ParticleSystem()->GetCurMaterial();
		if (GetTarget()->ParticleSystem()->IsDynamicMtrl())
		{
			m_bDynamicMtrl = true;
		}
		else
		{
			m_bDynamicMtrl = false;
		}
	}

	ComponentUI::render_update();
	if (!IsOpen())
		return;

	bool WorldSpawn = (bool)m_WorldSpawn;
	ImGui::Text("WorldSpawn"); ImGui::SameLine(); ImGui::Checkbox("##ParitlceWorldSpawn", &WorldSpawn);
	if (WorldSpawn)
		m_WorldSpawn = true;
	else
		m_WorldSpawn = false;


	ImGui::Text("Select-Texture"); 
	ImGui::SameLine();
	ImGui::InputText("##TexName", (char*)m_TexKey.data(), m_TexKey.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if(ImGui::Button("##TextureButton", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서 ListUI에 전달
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
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetTexture);

		pListUI->Open();
	}

	ImGui::Text("CSKey         ");
	ImGui::SameLine();
	m_CSComboBox->render_update();


	ImGui::Text("MaxCount      "); ImGui::SameLine(); ImGui::InputInt("##ParticleMaxCount", &m_iMaxCount, 0, 0, ImGuiFocusedFlags_None);
	ImGui::Text("AliveCount    "); ImGui::SameLine(); ImGui::InputInt("##ParticleAliveCount", &m_iAliveCount, 0, 0, ImGuiFocusedFlags_None);

	ImGui::Text("StartScale    "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleStartScale", m_vStartScale);
	ImGui::Text("EndScale      "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleEndScale", m_vEndScale);

	ImGui::Text("Min-Max Speed "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleMinMaxSpeed", m_vMinMaxSpeed);
	ImGui::Text("Min-Max Life  "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleMinMaxLife", m_vMinMaxLifeTime);
	ImGui::Text("SpawnRange    "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleSpawnRange", m_fSpawnRange);

	ImGui::Text("Frequency     "); ImGui::SameLine(); ImGui::InputFloat("##ParticleFrequency", &m_Frequency);
	ImGui::Text("AccTime       "); ImGui::SameLine(); ImGui::InputFloat("##ParticleAccTime", &m_fAccTime);


	// color select 
	static bool IsStartColor = true;
	if (ImGui::RadioButton("Start Color", IsStartColor)) { IsStartColor = true; }
	if (ImGui::RadioButton("End Color", !IsStartColor)) { IsStartColor = false; }

	Vec4& color = (IsStartColor) ? m_vStartColor : m_vEndColor;

	float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##0", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##1", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	if (GetTarget())
	{
		GetTarget()->ParticleSystem()->SetMaxCount(m_iMaxCount);
		GetTarget()->ParticleSystem()->SetAliveCount(m_iAliveCount);
		GetTarget()->ParticleSystem()->SetScale(m_vStartScale, m_vEndScale);
		GetTarget()->ParticleSystem()->SetColor(m_vStartColor, m_vEndColor);
		GetTarget()->ParticleSystem()->SetSpeed(m_vMinMaxSpeed);
		GetTarget()->ParticleSystem()->SetLifeTime(m_vMinMaxLifeTime);
		GetTarget()->ParticleSystem()->SetSpawnRange(m_fSpawnRange);
		GetTarget()->ParticleSystem()->SetFrequency(m_Frequency);
		GetTarget()->ParticleSystem()->SetWorldSpawn(m_WorldSpawn);
	}

	ImGui::NewLine();
	if (m_bDynamicMtrl)
	{
		ParamUI::ShowShaderParam(m_Mtrl.Get());
	}
}

void ParticleSystemUI::SetCSKey(DWORD_PTR _CSKey)
{
	string strKey = (char*)_CSKey;
	GetTarget()->ParticleSystem()->SetUpdateCS(StrToWstr(strKey));
}


void ParticleSystemUI::MtrlBtn()
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
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetMaterial);

		pListUI->Open();
	}
}

void ParticleSystemUI::SetMaterial(DWORD_PTR _strMaterialKey)
{
	string strKey = (char*)_strMaterialKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstrKey);
	assert(nullptr != pMtrl);

	GetTarget()->ParticleSystem()->SetSharedMaterial(pMtrl);
	GetTarget()->ParticleSystem()->ClearDynamicMtrl();
}

void ParticleSystemUI::SetMesh(DWORD_PTR _strMeshKey)
{
	string strKey = (char*)_strMeshKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstrKey);
	assert(nullptr != pMesh);

	GetTarget()->ParticleSystem()->SetMesh(pMesh);
}