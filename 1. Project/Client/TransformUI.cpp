#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

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
	if (GetTarget())
	{
		m_vPos = GetTarget()->Transform()->GetRelativePos();
		m_vScale = GetTarget()->Transform()->GetRelativeScale();
		m_vRot = GetTarget()->Transform()->GetRelativeRotation();

		m_bIgnScale = GetTarget()->Transform()->IsIgnoreParentScale();
	}

	// UI::update();
	ComponentUI::update();
}

void TransformUI::render_update()
{
	// 타입 텍스트 버튼
	ComponentUI::render_update();
	if (GetTarget() == nullptr)
		return;

	// 움직일지 아닐지
	ImGui::Text("Move"); ImGui::SameLine(); ImGui::Checkbox("##Position", &m_bMove);

	// 값 표시
	ImGui::Text("Position"); ImGui::SameLine(); ImGui::InputFloat3("##Position", m_vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::InputFloat3("##Scale", m_vScale);

	m_vRot.ToDegree();
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::InputFloat3("##Rotation", m_vRot);

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

	CGameObject* pMainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
	float camScale = pMainCamera->Camera()->GetOrthographicScale();


	Vec3 vTargetProjScale = GetTarget()->Transform()->GetWorldScale();
	vTargetProjScale /= camScale;

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	Vec2 vMouseViewPos = CKeyMgr::GetInst()->GetMouseViewPos();

	const Matrix viewMat = pMainCamera->Camera()->GetViewMat();

	Vec3 vTargetProjPos = GetTarget()->Transform()->GetWorldPos();
	Vec4 vTargetViewPos = MulMatrix(Vec4(vTargetProjPos, 1.f), viewMat);
	vTargetProjPos = Vec3(vTargetViewPos.x, vTargetViewPos.y, vTargetViewPos.z);
	vTargetProjPos /= camScale;


	// Tile Pressed
	if (KEY_PRESSED(KEY::LBTN) && m_bMove &&
		vTargetProjPos.x + vTargetProjScale.x / 2.f > vMouseViewPos.x &&
		vTargetProjPos.x - vTargetProjScale.x / 2.f < vMouseViewPos.x &&
		vTargetProjPos.y + vTargetProjScale.y / 2.f > vMouseViewPos.y &&
		vTargetProjPos.y - vTargetProjScale.y / 2.f < vMouseViewPos.y)
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		Vec3 vTargetPos = GetTarget()->Transform()->GetRelativePos();
		GetTarget()->Transform()->SetRelativePos(Vec3(vTargetPos.x + vMouseDir.x * camScale, vTargetPos.y + vMouseDir.y * camScale, vTargetPos.z ));
	}
}


