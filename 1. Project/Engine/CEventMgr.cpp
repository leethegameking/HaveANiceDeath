﻿#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CResMgr.h"

#include "CCollisionMgr.h"
#include "CCollider2D.h"
#include "CObjectManager.h"

LOAD_LEVEL CEventMgr::Load_Level_Func = nullptr;

CEventMgr::CEventMgr()
	: m_bLevelChanged(false)
	, m_bResChanged(false)
	, m_bResChangeFlag(false)
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::tick()
{
	m_bLevelChanged = false;
	m_bResChanged = false;
	// 메모리 정리
	for (size_t i = 0; i < m_vecGarbage.size(); ++i)
	{		
		delete m_vecGarbage[i];
	}
	m_vecGarbage.clear();

	for (size_t i = 0; i < m_vecGrave.size(); ++i)
	{
		m_vecGrave[i]->Collider2D()->Deactivate();
		m_vecGrave[i]->m_bGrave = false;
	}
	m_vecGrave.clear();


	// 이벤트 처리
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{

		case EVENT_TYPE::CREATE_OBJECT:
		{
			m_bLevelChanged = true;
			// wParam : GameObject Adress
			// lParam : Layer Index
			CGameObject* pNewObj = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;

			if (iLayerIdx == 0)
			{
				iLayerIdx = pNewObj->GetLayerIdx();
			}

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
			pLevel->AddGameObject(pNewObj, iLayerIdx);

			if( CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY )
				pNewObj->begin();
		}
			break;

		case EVENT_TYPE::DELETE_OBJECT:
		{
			m_bLevelChanged = true;
			// wParam : GameObject Adress
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;

			if (!pObj->IsDead())
			{				
				// 삭제처리할 최상위 부모만 가비지에 넣는다.
				m_vecGarbage.push_back(pObj);

				// 삭제할 오브젝트 포함, 모든 자식오브젝트를 Dead 체크한다.
				static list<CGameObject*> queue;				
				queue.push_back(pObj);
				while (!queue.empty())
				{
					CGameObject* pObj = queue.front();
					queue.pop_front();

					const vector<CGameObject*>& vecChild = pObj->GetChildObject();
					for (size_t i = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
						m_vecGarbage.push_back(vecChild[i]);
					}

					pObj->m_bDead = true;
				}
			}
		}
			break;

		case EVENT_TYPE::BURRY_OBJECT:
		{
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;
			if (!pObj->IsGrave())
			{
				m_vecGrave.push_back(pObj);
				pObj->m_bGrave = true;
			}
		}
			break;

		case EVENT_TYPE::ADD_CHILD:
		{
			m_bLevelChanged = true;

			CGameObject* pChildObj = (CGameObject*)m_vecEvent[i].wParam;
			CGameObject* pParentObj = (CGameObject*)m_vecEvent[i].lParam;

			pParentObj->AddChild(pChildObj);
		}
			break;

		case EVENT_TYPE::CHILD_TO_PARENT:
		{
			// wParam : ChildObj
			CGameObject* pChildObj = (CGameObject*)m_vecEvent[i].wParam;

			// 부모와 끊기
			pChildObj->DisconnectFromParent();

			// Layer 부모 오브젝트에 등록하기
			CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(pChildObj, pChildObj->GetLayerIdx());
			LevelChangFlagOn();
		}
			break;

		case EVENT_TYPE::CHANGE_LEVEL:
		{
			// wParam : string Change Level Relative Path
			char* ChangeLevelPath = (char*)m_vecEvent[i].wParam;
			
			wstring wstrLevelPath = StrToWstr(string(ChangeLevelPath));

			CLevel* pChangeLevel = Load_Level_Func(wstrLevelPath);

			CLevelMgr::GetInst()->ChangeLevel(pChangeLevel);

			LevelChangFlagOn();
		}
			break;

		case EVENT_TYPE::CHANGE_LEVEL_STATE:
		{
			// wParam : Level State
			CLevelMgr::GetInst()->ChangeLevelState((LEVEL_STATE)m_vecEvent[i].wParam);
			LevelChangFlagOn();
		}
			break;

		case EVENT_TYPE::DELETE_RES:
		{
			// wParam : RES_TYPE
			// lParam : Res Address
			CResMgr::GetInst()->DeleteRes(RES_TYPE(m_vecEvent[i].wParam), ((CRes*)m_vecEvent[i].lParam)->GetKey());
			ResChangeFlagOn();
		}
		break;

		case EVENT_TYPE::CHANGE_LAYER:
		{
			// wParam : GameObj
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;
			UINT idx = (UINT)m_vecEvent[i].lParam;
			CLevelMgr::GetInst()->GetCurLevel()->ChangeLayer(pObj, idx);
			LevelChangFlagOn();
		}
		break;

		case EVENT_TYPE::ARRANGE_COLLISION_LAYER:
		{
			// wParam : pair<int, int> layer1, layer2
			// lParam : Check or Release
			std::pair<int, int>* pairLayers = (std::pair<int, int>*)m_vecEvent[i].wParam;
			 bool bCheck = (bool)m_vecEvent[i].lParam;
			 // 원래 Check 되어 있었다면
			 if (bCheck)
			 {
				 CCollisionMgr::GetInst()->CollisionLayerRelease(pairLayers->first, pairLayers->second);
			 }
			 else
			 {
				 CCollisionMgr::GetInst()->CollisionLayerCheck(pairLayers->first, pairLayers->second);
			 }
		}
		break;

		case EVENT_TYPE::END:
			break;
		default:
			break;
		}
	}

	m_vecEvent.clear();

	if (m_bLevelChangeFlag)
	{
		m_bLevelChanged = true;
		m_bLevelChangeFlag = false;
	}

	// Res 바뀌었을 경우 이벤트 매니저와서 바뀌었다는걸 알려줌. -> 나중에 이벤트로 변경.
	if (m_bResChangeFlag)
	{
		m_bResChanged = true;
		m_bResChangeFlag = false;
	}
}