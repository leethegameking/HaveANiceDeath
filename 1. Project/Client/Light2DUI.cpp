#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>
#include <Engine/CTransform.h>

#include "CommonUI.h"

Light2DUI::Light2DUI()
	:ComponentUI("Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::update()
{

	ComponentUI::update();
}

void Light2DUI::render_update()
{
	if (GetTarget())
	{
		tLightInfo tInfo = GetTarget()->Light2D()->GetLightInfo();

		m_vDiff = tInfo.vDiff;

		m_vWorldPos = tInfo.vWorldPos;
		m_vWorldDir = tInfo.vWorldDir;
		m_iLightType = tInfo.iLightType;
		m_fRadius = tInfo.fRadius;
		m_fAngle = tInfo.fAngle;
	}


	ComponentUI::render_update();
	if (!IsOpen())
		return;

	ImGui::Text("LightType");
	ImGui::SameLine();
	int radio_bool = (int)m_iLightType;
	if (ImGui::RadioButton("Directional", radio_bool == (int)LIGHT_TYPE::DIRECTIONAL)) { radio_bool = (int)LIGHT_TYPE::DIRECTIONAL; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Point", radio_bool == (int)LIGHT_TYPE::POINT)) { radio_bool = (int)LIGHT_TYPE::POINT; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Spotlight", radio_bool == (int)LIGHT_TYPE::SPOT)) { radio_bool = (int)LIGHT_TYPE::SPOT; }

	ImGui::Text("Diffuse Color");
	CommonUI::ColorPicker(m_vDiff);
	ImGui::NewLine();

	ImGui::Text("Direction");
	ImGui::SameLine();
	Vec3 vRelativeRot = GetTarget()->Transform()->GetRelativeRotation();
	vRelativeRot.ToDegree();
	ImGui::InputFloat("##LightDir", &vRelativeRot.z);

	ImGui::Text("Radius   ");
	ImGui::SameLine();
	ImGui::InputFloat("##LightRadius", &m_fRadius);

	ImGui::Text("Angle    ");
	ImGui::SameLine();
	m_fAngle = m_fAngle / XM_PI * 180.f;
	ImGui::InputFloat("##LightAngle", &m_fAngle);

	GetTarget()->Light2D()->SetLightType((LIGHT_TYPE)radio_bool);

	vRelativeRot.ToRadian();
	GetTarget()->Transform()->SetRelativeRotation(vRelativeRot);
	m_fAngle = m_fAngle * XM_PI / 180.f;
	GetTarget()->Light2D()->SetAngle(m_fAngle);
	GetTarget()->Light2D()->SetRadius(m_fRadius);
	GetTarget()->Light2D()->SetLightColor(m_vDiff);
}

