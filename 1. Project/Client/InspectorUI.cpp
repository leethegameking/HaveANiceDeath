#include "pch.h"
#include "InspectorUI.h"

#include "ComponentUI.h"
#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"
#include "Collider2DUI.h"
#include "ParticleSystemUI.h"
#include "TileMapUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI{}
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 400.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 500.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);
}

InspectorUI::~InspectorUI()
{
}

void InspectorUI::update()
{
	if (!IsValid(m_TargetObj))
	{
		SetTarget(nullptr);
	}

	UI::update();
}

void InspectorUI::render_update()
{
	// 타겟 이름
	if (m_TargetObj)
	{
		string strObjName = string(m_TargetObj->GetName().begin(), m_TargetObj->GetName().end());
		ImGui::Text(strObjName.c_str());
		ImGui::Separator();
	}
}

void InspectorUI::SetTarget(CGameObject* _Target)
{
	m_TargetObj = _Target;
	

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		// 임시 if 아직 모든 컴포넌트에 대응 X
		if (m_arrComUI[i])
		{
			if (m_TargetObj)
			{
				if (m_TargetObj->GetComponent((COMPONENT_TYPE)i))
				{
					m_arrComUI[i]->SetTarget(m_TargetObj);
					m_arrComUI[i]->init();
					m_arrComUI[i]->Open(); // TargetObject와 Component를 보장.
				}
				else
				{
					m_arrComUI[i]->SetTarget(nullptr);
					m_arrComUI[i]->Close();
				}
			}
			else
			{
				m_arrComUI[i]->SetTarget(nullptr);
				m_arrComUI[i]->Close();
			}
		}
	}

}






