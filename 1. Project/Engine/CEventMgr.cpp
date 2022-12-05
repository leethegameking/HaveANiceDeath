#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"


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

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
			pLevel->AddGameObject(pNewObj, iLayerIdx);			
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
					}

					pObj->m_bDead = true;
				}
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
		case EVENT_TYPE::CHANGE_LEVEL:
		{
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::END:
			break;
		default:
			break;
		}
	}

	m_vecEvent.clear();

	// Res 바뀌었을 경우
	if (m_bResChangeFlag)
	{
		m_bResChanged = true;
		m_bResChangeFlag = false;
	}
}