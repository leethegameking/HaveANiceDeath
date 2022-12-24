#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CMeshRender.h"
#include "CScript.h"


#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CEventMgr.h"

CGameObject::CGameObject()
	: m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bGrave(false)
	, m_bDead(false)
{

}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _origin.m_arrCom[i])
			continue;

		AddComponent(_origin.m_arrCom[i]->Clone());		
	}
	
	for (size_t i = 0; i < _origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}	
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScripts);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	// Components
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	// Scripts
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	// Comopnent
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->tick();
	}

	// Scripts
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		if(m_vecScripts[i]->IsActive())
			m_vecScripts[i]->tick();
	}	

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->finaltick();
	}

	// Child Object
	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Register Layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);
}

void CGameObject::render()
{
	if (nullptr == m_pRenderComponent)
		return;
	if(m_pRenderComponent->IsActive())
		m_pRenderComponent->render();
}

void CGameObject::DisconnectFromParent()
{
	vector<CGameObject*>& vecChild = m_pParent->m_vecChild;
	
	vector<CGameObject*>::iterator iter = vecChild.begin();
	for(; iter != vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			vecChild.erase(iter);
			m_pParent = nullptr;

			return;
		}
	}

	// 자식벡터에 자식이 없었음.
	assert(nullptr);
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eComType = _pComponent->GetType();

	// 스크립트가 아닌 경우
	if (COMPONENT_TYPE::SCRIPT != eComType)
	{
		if (m_arrCom[(UINT)eComType])
			return;
		// assert(!m_arrCom[(UINT)eComType]);

		// 입력된 Component 가 RenderComponent 라면
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);
		if (nullptr != pRenderCom )
		{
			assert(!m_pRenderComponent); // render 기능 컴포넌트는 한개만 가질 수 있다.
			m_pRenderComponent = pRenderCom;
		}

		// GameObject 와 Component 가 서로를 가리킴
		_pComponent->m_pOwnerObject = this;
		m_arrCom[(UINT)eComType] = _pComponent;
	}
	
	// Script 인 경우
	else
	{
		_pComponent->m_pOwnerObject = this;
		m_vecScripts.push_back((CScript*)_pComponent);		
	}

	CEventMgr::GetInst()->LevelChangFlagOn();
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	if (_pChild->GetParent())
	{
		_pChild->DisconnectFromParent();
	}
	else
	{
		// 막 생성됨과 같은 이유로 레이어 미정인 상태의 오브젝트
		if (_pChild->m_iLayerIdx == -1)
		{
			_pChild->m_iLayerIdx = m_iLayerIdx;
		}
		else
		{
			CLayer* pChildLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(_pChild->m_iLayerIdx);
			pChildLayer->DeregisterObject(_pChild);
		}
	}


	_pChild->m_pParent = this;
	m_vecChild.push_back(_pChild);
}

#include "CEventMgr.h"
void CGameObject::Destroy()
{
	tEvent eve = {};
	eve.eType = EVENT_TYPE::DELETE_OBJECT;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}

