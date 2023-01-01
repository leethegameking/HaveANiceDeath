#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

SAVE_GAMEOBJECT CPrefab::Save_GameObject_Func = nullptr;
LOAD_GAMEOBJECT CPrefab::Load_GameObject_Func = nullptr;


CPrefab::CPrefab()
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _pProto)
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(_pProto)
{
    if (-1 != _pProto->GetLayerIdx())
    {
        m_pProtoObj = _pProto->Clone();
        m_pProtoObj->SetName(_pProto->GetName());
    }
}

CPrefab::~CPrefab()
{
    if (nullptr != m_pProtoObj)
        delete m_pProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
    if (nullptr == m_pProtoObj)
        return nullptr;
    CGameObject* pCloneObj = m_pProtoObj->Clone();
    pCloneObj->SetName(pCloneObj->GetName() + L"_clone");
    return pCloneObj;
}

void CPrefab::Save(const wstring& _strRelativePath)
{
    if (!CheckRelativePath(_strRelativePath))
    {
        assert(nullptr);
        return;
    }

    FILE* pFile = nullptr;

    wstring strFilePath = CONTENTPATH;
    strFilePath += _strRelativePath;

    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    // save key, path
    SaveKeyPath(pFile);

    // save protoobj info
    Save_GameObject_Func(m_pProtoObj, pFile);

    fclose(pFile);
}

int CPrefab::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    // load key path
    LoadKeyPath(pFile);


    m_pProtoObj = Load_GameObject_Func(pFile);
    
    fclose(pFile);

    return S_OK;
}