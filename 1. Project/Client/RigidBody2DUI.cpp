#include "pch.h"
#include "RigidBody2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CRigidbody2D.h>

RigidBody2DUI::RigidBody2DUI()
	:ComponentUI("RigidBody2DUI", COMPONENT_TYPE::RIGIDBODY2D)
{
}

RigidBody2DUI::~RigidBody2DUI()
{
}

void RigidBody2DUI::update()
{
}

void RigidBody2DUI::render_update()
{
	ComponentUI::render_update();

	CRigidbody2D* pRB = GetTarget()->Rigidbody2D();


	ImGui::Checkbox("Is Ground", &(pRB->m_bGround)); ImGui::SameLine(); ImGui::Checkbox("Ign Gravity", &(pRB->m_bIgnoreGravity));
	ImGui::Text("Gravity         "); ImGui::SameLine(); ImGui::InputFloat("##Gravity", &(pRB->m_fGravity));

	ImGui::Text("Force           "); ImGui::SameLine(); ImGui::InputFloat2("##Force", pRB->m_vForce);

	ImGui::Text("Speed           "); ImGui::SameLine(); ImGui::InputFloat2("##Speed", pRB->m_vSpeed);
	ImGui::Text("MaxSpeed        "); ImGui::SameLine(); ImGui::InputFloat("##MaxSpeed", &(pRB->m_fMaxSpeed));
	ImGui::Text("MaxGravitySpeed "); ImGui::SameLine(); ImGui::InputFloat("##MaxGravitySpeed", &(pRB->m_fMaxGravitySpeed));
	
	ImGui::Text("Dir             "); ImGui::SameLine(); ImGui::InputScalar("##Speed", ImGuiDataType_S8, &(pRB->m_sDir));
	ImGui::Text("PrevPos         "); ImGui::SameLine(); ImGui::InputFloat3("##PrevPos", pRB->m_vPrevPos);


}


