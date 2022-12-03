#include "pch.h"
#include "ParticleSystemUI.h"

#include "ListUI.h"
#include "CImGuiMgr.h"

#include <Engine/CParticleSystem.h>
#include <Engine/CResMgr.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::SetTexture(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	GetTarget()->ParticleSystem()->SetTexture(wstrKey);
}

void ParticleSystemUI::update()
{
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

		m_Frequency = GetTarget()->ParticleSystem()->GetFrequency();    //
		m_fAccTime = GetTarget()->ParticleSystem()->GetAccTime();     //

		m_WorldSpawn = GetTarget()->ParticleSystem()->GetWorldSpawn();


		wstring texKey = GetTarget()->ParticleSystem()->GetTexKey();
		m_TexKey = string(texKey.begin(), texKey.end());
	}

	ComponentUI::update();
}

void ParticleSystemUI::render_update()
{
	ComponentUI::render_update(); ImGui::SameLine(); 
	bool WorldSpawn = (bool)m_WorldSpawn;
	ImGui::Text("WorldSpawn"); ImGui::SameLine(); ImGui::Checkbox("##ParitlceWorldSpawn", &WorldSpawn);

	ImGui::Text("Select-Texture"); 
	ImGui::SameLine();
	ImGui::InputText("##TexName", (char*)m_TexKey.data(), m_TexKey.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if(ImGui::Button("##TextureButton", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// �޽� ����� �޾ƿͼ� ListUI�� ����
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		// �ʿ��� �̸��� �̾Ƽ� ���Ϳ� ��������.
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		// �̸� ItemList�� ����
		pListUI->SetItemList(vecRes);

		// Item�� DoubleClick�Ǿ����� �� ��ü�� SetMesh�Լ��� ȣ���Ѵ�. 
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetTexture);

		pListUI->Open();
	}

	if (WorldSpawn)
		m_WorldSpawn = true;
	else
		m_WorldSpawn = false;

	ImGui::Text("MaxCount      "); ImGui::SameLine(); ImGui::InputInt("##ParticleMaxCount", &m_iMaxCount, 0, 0, ImGuiFocusedFlags_None);
	ImGui::Text("AliveCount    "); ImGui::SameLine(); ImGui::InputInt("##ParticleAliveCount", &m_iAliveCount, 0, 0, ImGuiFocusedFlags_None);

	ImGui::Text("StartScale    "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleStartScale", m_vStartScale);
	ImGui::Text("EndScale      "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleEndScale", m_vEndScale);

	static bool IsStartColor = false;

	ImGui::Text("Is StartColor?");
	ImGui::SameLine();
	ImGui::Checkbox("##ColorCheck", &IsStartColor);

	Vec4& color = (IsStartColor) ? m_vStartColor : m_vEndColor;

	float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##0", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##1", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	ImGui::Text("Min-Max Speed "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleMinMaxSpeed", m_vMinMaxSpeed);
	ImGui::Text("Min-Max Life  "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleMinMaxLife", m_vMinMaxLifeTime);
	ImGui::Text("SpawnRange    "); ImGui::SameLine(); ImGui::InputFloat2("##ParticleSpawnRange", m_fSpawnRange);

	ImGui::Text("Frequency     "); ImGui::SameLine(); ImGui::InputFloat("##ParticleFrequency", &m_Frequency);
	ImGui::Text("AccTime       "); ImGui::SameLine(); ImGui::InputFloat("##ParticleAccTime", &m_fAccTime);

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
}


