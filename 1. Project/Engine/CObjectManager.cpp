#include "pch.h"
#include "CObjectManager.h"

#include "CGameObject.h"

CObjectManager::CObjectManager()
	: m_vecAllObject()
{
	m_vecAllObject.reserve(1000);
}

CObjectManager::~CObjectManager()
{

}

void CObjectManager::RegisterObject(CGameObject* _pObj)
{
	m_vecAllObject.push_back(_pObj);
}

CGameObject* CObjectManager::FindObjByName(wstring _name)
{
	for (size_t i = 0; i < m_vecAllObject.size(); ++i)
	{
		if (_name == m_vecAllObject[i]->GetName())
		{
			return m_vecAllObject[i];
		}
	}

	return nullptr;
}

void CObjectManager::Clear()
{
	m_vecAllObject.clear();
}
