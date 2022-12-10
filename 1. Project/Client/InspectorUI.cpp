#include "pch.h"
#include "InspectorUI.h"

#include "ComponentUI.h"
#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"
#include "Collider2DUI.h"
#include "ParticleSystemUI.h"
#include "TileMapUI.h"
#include "Animator2DUI.h"
#include "MaterialUI.h"

#include "MeshUI.h"
#include "ComputeShaderUI.h"
#include "GraphicsShaderUI.h"

#include "Animation2DUI.h"
#include "ComboBox.h"


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

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 500.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);


	//==================================================================================

	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]->SetSize(Vec2(0.f, 100.f));
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]->SetSize(Vec2(0.f, 100.f));
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(Vec2(0.f, 100.f));
	m_arrResUI[(UINT)RES_TYPE::MESH]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::ANIMATION2D] = new Animation2DUI;
	m_arrResUI[(UINT)RES_TYPE::ANIMATION2D]->SetSize(Vec2(0.f, 500.f));
	m_arrResUI[(UINT)RES_TYPE::ANIMATION2D]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::ANIMATION2D]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);

	//==================================================================================

	m_CompComboBox = new ComboBox;

	vector<string> ComponentName;
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		ComponentName.push_back(COMPONENT_TYPE_CHAR[i]);
	}
	m_CompComboBox->init(ComponentName, 0);
	m_CompComboBox->AddSelectedFunc_ReturnInt(this, FUNC_1(&InspectorUI::AddComponent));
}

InspectorUI::~InspectorUI()
{
	if(m_CompComboBox)
		delete m_CompComboBox;
}

void InspectorUI::update()
{
	if (!IsValid(m_TargetObj))
	{
		SetTargetObj(nullptr);
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

void InspectorUI::last_render()
{
	ImGui::Text("Add Component");
	ImGui::SameLine();
	if(m_TargetObj)
		m_CompComboBox->render_update();
}

#include <Engine/GlobalComponent.h>
#define AddComp(eType) m_TargetObj->AddComponent(new eType);
void InspectorUI::AddComponent(DWORD_PTR _idx)
{
	int idx = _idx;
	COMPONENT_TYPE eType = (COMPONENT_TYPE)idx;

	switch (eType)
	{
	case COMPONENT_TYPE::TRANSFORM:
		AddComp(CTransform);
		break;
	case COMPONENT_TYPE::CAMERA:
		AddComp(CCamera);
		break;
	case COMPONENT_TYPE::COLLIDER2D:
		AddComp(CCollider2D);
		break;
	case COMPONENT_TYPE::COLLIDER3D:
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
		AddComp(CAnimator2D);
		break;
	case COMPONENT_TYPE::ANIMATOR3D:
		break;
	case COMPONENT_TYPE::LIGHT2D:
		AddComp(CLight2D);
		break;
	case COMPONENT_TYPE::LIGHT3D:
		break;
	case COMPONENT_TYPE::MESHRENDER:
		break;
	case COMPONENT_TYPE::TILEMAP:
		AddComp(CTileMap);
		break;
	case COMPONENT_TYPE::PARTICLESYSTEM:
		AddComp(CParticleSystem);
		break;
	case COMPONENT_TYPE::SKYBOX:
		break;
	case COMPONENT_TYPE::DECAL:
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		break;
	case COMPONENT_TYPE::END:
		break;
	case COMPONENT_TYPE::SCRIPT:
		break;
	default:
		break;
	}

	SetTargetObj(m_TargetObj);
}

void InspectorUI::SetTargetObj(CGameObject* _Target)
{
	if (_Target)
		SetTargetRes(nullptr);

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

void InspectorUI::SetTargetRes(Ptr<CRes> _Res)
{
	if (_Res.Get())
		SetTargetObj(nullptr);

	if (_Res.Get())
	{
		if (m_TargetRes.Get() && m_arrResUI[(UINT)m_TargetRes->GetResType()])
		{
			m_arrResUI[(UINT)m_TargetRes->GetResType()]->Close();
		}

		m_TargetRes = _Res;
		RES_TYPE eType = m_TargetRes->GetResType();

		if (m_arrResUI[(UINT)eType])
		{
			m_arrResUI[(UINT)eType]->SetTarget(m_TargetRes);
			m_arrResUI[(UINT)eType]->init();
			m_arrResUI[(UINT)eType]->Open();
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			if (m_arrResUI[i])
			{
				m_arrResUI[i]->SetTarget(nullptr);
				m_arrResUI[i]->Close();
			}
		}
	}

}






