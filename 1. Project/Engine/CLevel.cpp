#include "pch.h"
#include "CLevel.h"


CLevel::CLevel()
	: m_State(LEVEL_STATE::STOP)
{
	m_arrLayer[0].SetName(L"Default");

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_iLayerIdx = i;
	}
}

CLevel::~CLevel()
{

}


void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].begin();
	}
}

void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].finaltick();
	}
}

void CLevel::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].clear();
	}
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i].GetName())
			return &m_arrLayer[i];
	}
	
	return nullptr;
}

void CLevel::AddGameObject(CGameObject* _pObject, UINT _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);

	m_arrLayer[_iLayerIdx].AddGameObject(_pObject);
}

void CLevel::AddGameObject(CGameObject* _pObject, const wstring& _strLayerName)
{
	CLayer* pLayer = GetLayer(_strLayerName);

	assert(pLayer);

	pLayer->AddGameObject(_pObject);
}


#include "CGameObject.h"
void CLevel::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_strName == vecObjects[j]->GetName())
				_out.push_back(vecObjects[j]);
		}
	}
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name == vecObjects[j]->GetName())
				return vecObjects[j];
		}
	}

	return nullptr;
}

// Layer내의 Vector idx 반환
size_t CLevel::FindObjectIdx(CGameObject* _pObj)
{
	const vector<CGameObject*>& vecObjects = GetLayer(_pObj->GetLayerIdx())->GetObjects();

	for (size_t j = 0; j < vecObjects.size(); ++j)
	{
		if (_pObj == vecObjects[j])
			return  j;
	}
}



void CLevel::ChangeLayer(CGameObject* _srcObj, UINT _layerIdx)
{
	CLayer* pLayer = GetLayer(_srcObj->GetLayerIdx());

	// 부모일때만
	if (_srcObj->GetParent() == nullptr)
	{
		pLayer->DeregisterObject(_srcObj);
		AddGameObject(_srcObj, _layerIdx);
	}

	_srcObj->SetLayerIdx(_layerIdx);

}
