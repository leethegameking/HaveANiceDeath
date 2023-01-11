#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CDevice.h>


TransformUI::TransformUI()
	: ComponentUI("TransfromUI", COMPONENT_TYPE::TRANSFORM)
	, m_bIgnScale(false)
	, m_bMove(false)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::update()
{


	// UI::update();
	ComponentUI::update();
}

void TransformUI::render_update()
{

	// 타입 텍스트 버튼
	ComponentUI::render_update();
	if (GetTarget() == nullptr)
		return;

	if (GetTarget())
	{
		m_vPos = GetTarget()->Transform()->GetRelativePos();
		m_vScale = GetTarget()->Transform()->GetRelativeScale();
		m_vRot = GetTarget()->Transform()->GetRelativeRotation();

		m_bIgnScale = GetTarget()->Transform()->IsIgnoreParentScale();
	}

	// 움직일지 아닐지
	ImGui::Text("Move"); ImGui::SameLine(); ImGui::Checkbox("##Position", &m_bMove);

	// 값 표시
	ImGui::Text("Position"); ImGui::SameLine(); ImGui::InputFloat3("##Position", m_vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::InputFloat3("##Scale", m_vScale);

	m_vRot.ToDegree();
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::InputFloat3("##Rotation", m_vRot);
	ImGui::Text("World - Position"); ImGui::SameLine(); ImGui::InputFloat3("##PositionWorld", GetTarget()->Transform()->GetWorldPos());
	ImGui::Text("World - Scale   "); ImGui::SameLine(); ImGui::InputFloat3("##ScaleWorld", GetTarget()->Transform()->GetWorldScale());

	m_vRot.ToDegree();
	ImGui::Text("World - Rotation"); ImGui::SameLine(); ImGui::InputFloat3("##RotationWorld", GetTarget()->Transform()->GetWorldRotation());

	ImGui::Text("Ignore Parent Scale"); ImGui::SameLine(); ImGui::Checkbox("##IgnParentScaele", &m_bIgnScale);

	// 에디터 값으로 Transfrom값을 변경
	if (GetTarget())
	{
		GetTarget()->Transform()->SetRelativePos(m_vPos);
		GetTarget()->Transform()->SetRelativeScale(m_vScale);

		m_vRot.ToRadian();
		GetTarget()->Transform()->SetRelativeRotation(m_vRot);

		GetTarget()->Transform()->SetIgnoreParentScale(m_bIgnScale);
	}

	CCamera* pMainCamera = CRenderMgr::GetInst()->GetMainCam();
	Vec3 vCamPos = pMainCamera->Transform()->GetRelativePos();
	float vCamScale = pMainCamera->GetOrthographicScale();


	Vec3 vWorldScale = GetTarget()->Transform()->GetWorldScale();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	Vec2 vMouseDirectXPos = CKeyMgr::GetInst()->GetMouseDirectXPos();

	const Matrix viewMat = pMainCamera->Camera()->GetViewMat();

	Vec3 vWorldPos = GetTarget()->Transform()->GetWorldPos();


	// Tile Pressed
	if (KEY_PRESSED(KEY::LBTN) && m_bMove/*&&
		vWorldPos.x + vWorldScale.x / 2.f > (vMouseDirectXPos.x * vCamScale + vCamPos.x)  &&
		vWorldPos.x - vWorldScale.x / 2.f < (vMouseDirectXPos.x * vCamScale + vCamPos.x)  &&
		vWorldPos.y + vWorldScale.y / 2.f > (vMouseDirectXPos.y * vCamScale + vCamPos.y)  &&
		vWorldPos.y - vWorldScale.y / 2.f < (vMouseDirectXPos.y * vCamScale + vCamPos.y)*/  )
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		Vec3 vTargetPos = GetTarget()->Transform()->GetRelativePos();
		GetTarget()->Transform()->AddWorldPos(Vec3(vMouseDir.x * vCamScale, vMouseDir.y * vCamScale, vTargetPos.z ));
	}
}


