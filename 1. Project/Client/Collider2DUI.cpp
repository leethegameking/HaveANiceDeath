#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2DUI", COMPONENT_TYPE::COLLIDER2D)
	, IsCollisionColor(false)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::update()
{
	if (GetTarget())
	{
		m_vOffsetPos = GetTarget()->Collider2D()->GetOffsetPos();
		m_vScale = GetTarget()->Collider2D()->GetScale();
		m_vRot.z = GetTarget()->Collider2D()->GetRotationZ();

		m_vFinalPos = GetTarget()->Collider2D()->GetFinalPos();
		m_vFinalScale = GetTarget()->Collider2D()->GetFinalScale();
		m_bIgnorObjectScale = GetTarget()->Collider2D()->GetIgnoreObjectScale();

		m_vIdleColor = GetTarget()->Collider2D()->GetIdleColor();
		m_vCollisionColor = GetTarget()->Collider2D()->GetCollisionColor();
		m_iOverlapCount = GetTarget()->Collider2D()->GetOverlapCount();
	}

	ComponentUI::update();
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("Offset-Position"); ImGui::SameLine(); ImGui::InputFloat2("##OffsetPosition", m_vOffsetPos);
	ImGui::Text("Final-Position "); ImGui::SameLine(); ImGui::InputFloat2("##OffsetFinalPosition", m_vFinalPos);

	ImGui::Text("Scale          "); ImGui::SameLine(); ImGui::InputFloat2("##Scale", m_vScale);
	ImGui::Text("Final-Scale    "); ImGui::SameLine(); ImGui::InputFloat2("##FinalScale", m_vFinalScale);

	ImGui::Text("RotationZ      "); ImGui::SameLine(); ImGui::InputFloat("##OffsetRotationZ", &m_vRot.z, 0, 0, "%.3f", ImGuiInputTextFlags_None);
	ImGui::Text("Overlap-Count  "); ImGui::SameLine(); ImGui::InputInt("##OverlapCount", &m_iOverlapCount, 0, 0, ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Ignore Object Scale"); ImGui::SameLine(); ImGui::Checkbox("##IgnObjScale", &m_bIgnorObjectScale);


	ImGui::Text("Collider-Color"); 
	ImGui::SameLine();
	//ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 35.f, ImGui::GetCursorPos().y));
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 125.f, ImGui::GetCursorPos().y));
	ImGui::Checkbox("Collision", &IsCollisionColor);
	

	// 삼항연산자를 써서 레퍼런스가 어떤 것을 참조할지 결정.
	Vec4& color = (IsCollisionColor) ? m_vCollisionColor : m_vIdleColor;

	float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##0", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##MyColor##1", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	//ImGui::Text("Position"); ImGui::SameLine(); ImGui::InputFloat2("##OffsetPosition", fArr);
	//ImGui::Text("Position"); ImGui::SameLine(); ImGui::InputFloat2("##OffsetPosition", fArr);

	if (GetTarget())
	{
		GetTarget()->Collider2D()->SetOffsetPos(m_vOffsetPos);
		GetTarget()->Collider2D()->SetRotationZ(m_vRot.z);
		GetTarget()->Collider2D()->SetScale(m_vScale);
		GetTarget()->Collider2D()->SetIgnoreObjectScale(m_bIgnorObjectScale);
		GetTarget()->Collider2D()->SetIdleColor(m_vIdleColor);
		GetTarget()->Collider2D()->SetCollisionColor(m_vCollisionColor);
	}
}



