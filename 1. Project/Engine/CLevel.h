#pragma once

#include "CEntity.h"
#include "CLayer.h"

class CGameObject;

class CLevel :
    public CEntity 
{
private:
    CLayer          m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;

public:
    // 시점 함수
    void begin();
    void tick();
    void finaltick();
    
    void ClearLayer();

public:
    LEVEL_STATE GetState() { return m_State; }
    void SetState(LEVEL_STATE _state) { m_State = _state; }

    CLayer* GetLayer(UINT _iIdx) { return &m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

    void AddGameObject(CGameObject* _pObject, UINT _iLayerIdx);
    void AddGameObject(CGameObject* _pObject, const wstring& _strLayerName);

public:
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _out);
    CGameObject* FindObjectByName(const wstring& _Name);

public:
    CLONE_DUMMY(CLevel);

public:
    CLevel();
    virtual ~CLevel();
};

