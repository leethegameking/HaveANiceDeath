#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"


CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::tick()
{
	// �޸� ����
	for (size_t i = 0; i < m_vecGarbage.size(); ++i)
	{		
		delete m_vecGarbage[i];
	}
	m_vecGarbage.clear();


	// �̺�Ʈ ó��
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
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
			// wParam : GameObject Adress
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;

			if (!pObj->IsDead())
			{				
				// ����ó���� �ֻ��� �θ� �������� �ִ´�.
				m_vecGarbage.push_back(pObj);

				// ������ ������Ʈ ����, ��� �ڽĿ�����Ʈ�� Dead üũ�Ѵ�.
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
		case EVENT_TYPE::CHANGE_LEVEL:
			break;
		case EVENT_TYPE::END:
			break;
		default:
			break;
		}
	}

	m_vecEvent.clear();
}