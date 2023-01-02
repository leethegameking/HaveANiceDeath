#include "pch.h"
#include "CameraUI.h"
#include "ListUI.h"

#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CameraUI::CameraUI()
	: ComponentUI("CameraUI", COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::ORTHOGRAHPICS)
	, m_fFar(0.f)
	, m_fScale(0.f)
	, m_iLayerVisible(0)
	, m_iCamIdx(0)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		wstring layerName = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetName();
		string strLayerName = string(layerName.begin(), layerName.end());
		m_vecLayer.push_back("[" + to_string(i) + "]" + " " + strLayerName);
	}

	m_ListUI = new ListUI;
}

CameraUI::~CameraUI()
{
	if (nullptr != m_ListUI)
		delete m_ListUI;
}


void CameraUI::SetLayerVisible()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		vector<bool>& LayerIdxArr = m_ListUI->GetMultiSelectIdx();
		
		if(LayerIdxArr[i])
			GetTarget()->Camera()->SetLayerVisible(i);
		else
			GetTarget()->Camera()->SetLayerInvisible(i);
	}
}

void CameraUI::init()
{
	m_ListUI->SetMultiSelect(true);
	m_ListUI->AddDynamicSelected(this, (FUNC_0)&CameraUI::SetLayerVisible);
	m_iLayerVisible = GetTarget()->Camera()->GetLayerVisible();
	m_ListUI->init(m_vecLayer, m_iLayerVisible);
}

void CameraUI::update()
{
	if (GetTarget())
	{
		m_eProjType = GetTarget()->Camera()->GetProjType();
		m_fFar = GetTarget()->Camera()->GetFar();
		m_fScale = GetTarget()->Camera()->GetOrthographicScale();
		m_iLayerVisible = GetTarget()->Camera()->GetLayerVisible();
		m_iCamIdx = GetTarget()->Camera()->GetCamIdx();
	}

	// UI::update();
	ComponentUI::update();
}

void CameraUI::render_update()
{
	ComponentUI::render_update();
	if (!IsOpen())
		return;

	ImGui::Text("Projection Type ");
	ImGui::SameLine();
	static int e = 0;

	if (ImGui::RadioButton("ORTHOGRAHPICS", &e, 0))
	{
		m_eProjType = PROJ_TYPE::ORTHOGRAHPICS;
		GetTarget()->Camera()->SetProjType(m_eProjType);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PERSPECTIVE", &e, 1))
	{
		m_eProjType = PROJ_TYPE::PERSPECTIVE;
		GetTarget()->Camera()->SetProjType(m_eProjType);
	}

	ImGui::Text("Far            ");
	ImGui::SameLine();
	ImGui::InputFloat("##CamFar", &m_fFar);

	ImGui::Text("Scale          ");
	ImGui::SameLine();
	ImGui::InputFloat("##CamScale", &m_fScale);

	ImGui::Text("Cam-Order      ");
	ImGui::SameLine();
	ImGui::InputInt("##CamOrder", &m_iCamIdx, 0, 0, ImGuiInputTextFlags_None);

	ImGui::Text("Visible-Layer  ");
	ImGui::SameLine();
	if (GetTarget())
	{
		m_ListUI->render_update();
	}


	if (GetTarget())
	{
		GetTarget()->Camera()->SetFar(m_fFar);
		GetTarget()->Camera()->SetOrthographicScale(m_fScale);
		GetTarget()->Camera()->SetCamIdx(m_iCamIdx);
	}
}


