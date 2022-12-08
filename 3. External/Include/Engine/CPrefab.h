#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject*    m_pProtoObj;

public:
    CGameObject* Instantiate();


    // ���Ϸ� ���� �ε�
    virtual int Load(const wstring& _strFilePath) { return E_FAIL; }
    virtual void Save(const wstring& _strFilePath) override {};

    CLONE_DUMMY(CPrefab);

public:
    CPrefab();
    CPrefab(CGameObject* _pProto);
    ~CPrefab();
};

