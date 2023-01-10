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
#include "Light2DUI.h"
#include "SoundUI.h"
#include "RigidBody2DUI.h"


#include "MeshUI.h"
#include "ComputeShaderUI.h"
#include "GraphicsShaderUI.h"
#include "TextureUI.h"
#include "PrefabUI.h"

#include "Animation2DUI.h"
#include "ComboBox.h"


#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

#include <Engine/CScript.h>

#include <Script/CScriptMgr.h>

#include "ScriptUI.h"

#include "CImGuiMgr.h"

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
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 200.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D] = new RigidBody2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]);

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

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::SOUND] = new SoundUI;
	m_arrResUI[(UINT)RES_TYPE::SOUND]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::SOUND]);

	m_arrResUI[(UINT)RES_TYPE::PREFAB] = new PrefabUI;
	m_arrResUI[(UINT)RES_TYPE::PREFAB]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::PREFAB]);

	//==================================================================================

	m_CompComboBox = new ComboBox;

	vector<string> ComponentName;
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		ComponentName.push_back(COMPONENT_TYPE_CHAR[i]);
	}
	m_CompComboBox->init_not_res(ComponentName, 0);
	m_CompComboBox->AddSelectedFunc_ReturnInt(this, FUNC_1(&InspectorUI::AddComponent));



	m_ScriptComboBox = new ComboBox;

	vector<wstring> ScriptName;
	CScriptMgr::GetScriptInfo(ScriptName);

	vector<string> strScriptName;
	strScriptName.reserve(ScriptName.size());
	for (size_t i = 0; i < ScriptName.size(); ++i)
	{
		strScriptName.push_back(WstrToStr(ScriptName[i]));
	}

	m_ScriptComboBox->init_not_res(strScriptName, 0);
	m_ScriptComboBox->AddSelectedFunc_ReturnInt(this, FUNC_1(&InspectorUI::AddScript));

	m_ComboLayer = new ComboBox;

	m_ComboLayer->AddSelectedFunc_ReturnInt(this, FUNC_1(&InspectorUI::SetLayer));

	//===================================================================================

	ScriptUI* pScriptUI = new ScriptUI;
	pScriptUI->SetSize(Vec2(0.f, 200.f));
	pScriptUI->Close();
	AddChild(pScriptUI);
	m_vecScriptUI.push_back(pScriptUI);
}

InspectorUI::~InspectorUI()
{
	if(m_CompComboBox)
		delete m_CompComboBox;

	if (m_ScriptComboBox)
		delete m_ScriptComboBox;

	if (m_ComboLayer)
		delete m_ComboLayer;
}

void InspectorUI::update()
{
	// 타겟된 오브젝트가 삭제될 때. but Res가 타겟될때에도 들어오고 있음. -> 조건 추가해줌 !m_TargetRes.Get()
	if (!IsValid(m_TargetObj) && !m_TargetRes.Get())
	{
		SetTargetObj(nullptr);
	}

	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		static vector<string> vecLayerName;
		vecLayerName.clear();
		vecLayerName.reserve(MAX_LAYER);
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			string strLayerIdx = to_string(i);
			vecLayerName.push_back("(" + strLayerIdx + ") " + WstrToStr(pCurLevel->GetLayer(i)->GetName()));
		}
		if(m_TargetObj)
			m_ComboLayer->init_not_res(vecLayerName, m_TargetObj->GetLayerIdx());
		else
			m_ComboLayer->init_not_res(vecLayerName, 0);
	}

	UI::update();
}

void InspectorUI::render_update()
{
	// 타겟
	if (m_TargetObj)
	{
		string strObjName = string(m_TargetObj->GetName().begin(), m_TargetObj->GetName().end());
		ImGui::Text(strObjName.c_str());

		ImGui::SameLine();

		// LayerCombo 
		m_ComboLayer->SetCurIdx(m_TargetObj->GetLayerIdx());
		m_ComboLayer->render_update();

		ImGui::SameLine();
		if (ImGui::Button("##LayerNameSetBtn"))
		{
			CImGuiMgr::GetInst()->FindUI("LayerNameTool")->Open();
		}

		ImGui::Separator();
	}
}

void InspectorUI::last_render()
{
	if (m_TargetObj)
	{
		ImGui::Text("Add Component ");
		ImGui::SameLine();
		m_CompComboBox->render_update();

		ImGui::Text("Add Script    ");
		ImGui::SameLine();
		m_ScriptComboBox->render_update();
	}
}

void InspectorUI::SetTargetObj(CGameObject* _Target)
{
	if(_Target && m_TargetRes.Get() && !(m_TargetRes->GetResType() == RES_TYPE::PREFAB))
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

	// Script render
	if (nullptr == m_TargetObj)
	{
		for (auto& pScript : m_vecScriptUI)
		{
			pScript->Close();
		}
	}

	else
	{
		// if(m_vecScriptUI.empty())
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Close();
		}

		const vector<CScript*>& vecScript = m_TargetObj->GetScripts();

		if (m_vecScriptUI.size() < vecScript.size())
		{
		 	size_t AddScriptUI = vecScript.size() - m_vecScriptUI.size();
			for (size_t i = 0; i < AddScriptUI; ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				pScriptUI->SetSize(Vec2(0.f, 200.f));
				pScriptUI->Close();
				AddChild(pScriptUI);
				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			m_vecScriptUI[i]->Open();
			m_vecScriptUI[i]->SetTargetScript(vecScript[i]);
			m_vecScriptUI[i]->SetTarget(m_TargetObj);
		}
	}
}

void InspectorUI::SetTargetRes(Ptr<CRes> _Res)
{
	if (_Res.Get())
		SetTargetObj(nullptr);
	else
		m_TargetRes = nullptr;


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
	case COMPONENT_TYPE::ANIMATOR2D:
		AddComp(CAnimator2D);
		break;
	case COMPONENT_TYPE::LIGHT2D:
		AddComp(CLight2D);
		break;
	case COMPONENT_TYPE::MESHRENDER:
		AddComp(CMeshRender);
		break;
	case COMPONENT_TYPE::TILEMAP:
		AddComp(CTileMap);
		break;
	case COMPONENT_TYPE::PARTICLESYSTEM:
		AddComp(CParticleSystem);
		break;
	case COMPONENT_TYPE::RIGIDBODY2D:
		AddComp(CRigidbody2D);
		break;

	case COMPONENT_TYPE::SKYBOX:
		break;
	case COMPONENT_TYPE::DECAL:
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		break;
	case COMPONENT_TYPE::ANIMATOR3D:
		break;
	case COMPONENT_TYPE::COLLIDER3D:
		break;
	case COMPONENT_TYPE::LIGHT3D:
		break;
	case COMPONENT_TYPE::END:
		break;
	case COMPONENT_TYPE::SCRIPT:
		break;
	default:
		break;
	}

	// 갱신
	SetTargetObj(m_TargetObj);
}

void InspectorUI::AddScript(DWORD_PTR _idx)
{
	m_TargetObj->AddComponent(CScriptMgr::GetScript((int)_idx));

	// 갱신
	SetTargetObj(m_TargetObj);
}

void InspectorUI::SetLayer(DWORD_PTR _idx)
{
	if (m_TargetRes->GetResType() == RES_TYPE::PREFAB)
	{
		m_TargetObj->SetLayerIdx((int)_idx);
	}
	else
	{
		tEvent evn = {};
		evn.eType = EVENT_TYPE::CHANGE_LAYER;
		evn.wParam = (DWORD_PTR)m_TargetObj;
		evn.lParam = _idx;
		CEventMgr::GetInst()->AddEvent(evn);
	}
}



