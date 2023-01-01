#include "pch.h"
#include "CSaveLoadMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/GlobalComponent.h>
#include <Engine/CScript.h>
#include <Engine/CPrefab.h>


#include <Script/CScriptMgr.h>

bool CSaveLoadMgr::m_bFirstLoad = true;
bool CSaveLoadMgr::m_bPrefabLoad = false;

CSaveLoadMgr::CSaveLoadMgr()
{
}

CSaveLoadMgr::~CSaveLoadMgr()
{
}


void CSaveLoadMgr::init()
{
    CPrefab::Save_GameObject_Func = &CSaveLoadMgr::SaveGameObject;
    CPrefab::Load_GameObject_Func = &CSaveLoadMgr::LoadGameObject;
    CEventMgr::Load_Level_Func = &CSaveLoadMgr::LoadLevel;
}

void CSaveLoadMgr::SaveLevel(CLevel* _Level, wstring _strRelativePath)
{
    assert(_Level);

    // path
    wstring strFilePath = CONTENTPATH;
    strFilePath += _strRelativePath;

    // file 
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    // level name
    _Level->SaveToFile(pFile);

    // level -> layer
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // layer name
        CLayer* pLayer = _Level->GetLayer(i);
        pLayer->SaveToFile(pFile);

        // layer -> parent objs
        const vector<CGameObject*>& vecParentObj = pLayer->GetParentObjects();

        // parent obj count
        size_t iObjectCount = vecParentObj.size();
        fwrite(&iObjectCount, sizeof(size_t), 1, pFile);

        // parent obj
        for (size_t j = 0; j < iObjectCount; ++j)
        {
            SaveGameObject(vecParentObj[j], pFile);
        }
    }

    // CollisonMgr
    WORD* pCollisonMat = CCollisionMgr::GetInst()->GetCollisionMat();
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        fwrite(&pCollisonMat[i], sizeof(WORD), 1, pFile);
    }

    // Global LayerName
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        SaveWStringToFile(CLevelMgr::GetInst()->GetLayerName(i), pFile);
    }

    fclose(pFile);
}

void CSaveLoadMgr::SaveGameObject(CGameObject* _Object, FILE* _File)
{
    // obj name
    _Object->SaveToFile(_File);

    int iLayerIdx = _Object->GetLayerIdx();
    fwrite(&iLayerIdx, sizeof(int), 1, _File);

    // obj components
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pComponent = _Object->GetComponent(COMPONENT_TYPE(i));
        if (nullptr != pComponent)
        {
            pComponent->SaveToFile(_File); 
        }
    }

    // notify end of reading component
    COMPONENT_TYPE eComponentEnd = COMPONENT_TYPE::END;
    fwrite(&eComponentEnd, sizeof(COMPONENT_TYPE), 1, _File);

    // script
    const vector<CScript*>& vecScript = _Object->GetScripts();
    size_t iScriptCount = vecScript.size();
    // save script count 
    fwrite(&iScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        // save script name
        SaveWStringToFile(CScriptMgr::GetScriptName(vecScript[i]), _File);

        // save scipt member
        // 구현 안 되어 있다면 Entity에서 이름만 저장.
        vecScript[i]->SaveToFile(_File);
    }

    // child obj
    const vector<CGameObject*>& vecChild = _Object->GetChildObject();
    size_t iChildCount = vecChild.size();
    // save child size
    fwrite(&iChildCount, sizeof(size_t), 1, _File);

    // save child -> 재귀적으로 자식 저장
    for (size_t i = 0; i < vecChild.size(); ++i)
    {
        SaveGameObject(vecChild[i], _File);
    }
}


CLevel* CSaveLoadMgr::LoadLevel(wstring _strRelativePath)
{
    // path
    wstring strFilePath = CONTENTPATH;
    strFilePath += _strRelativePath;

    // file
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

    // level name
    CLevel* pLevel = new CLevel;
    pLevel->LoadFromFile(pFile);

    // level -> layer
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // layer name
        CLayer* pLayer = pLevel->GetLayer(i);
        pLayer->LoadFromFile(pFile);

        // parent obj count
        size_t iObjectCount = 0;
        fread(&iObjectCount, sizeof(size_t), 1, pFile);

        // parent obj
        for (size_t j = 0; j < iObjectCount; ++j)
        {
            CGameObject* pObject = LoadGameObject(pFile);
            pLayer->AddGameObject(pObject);
        }

    }

    // CollisonMgr
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        WORD pMatrixElement;
        fread(&pMatrixElement, sizeof(WORD), 1, pFile);

        CCollisionMgr::GetInst()->SetCollisionMat(i, pMatrixElement);
    }

    // Global LayerName
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        if (m_bFirstLoad)
        {
            wstring strLayerName;
            LoadWStringFromFile(strLayerName, pFile);
            CLevelMgr::GetInst()->SetLayerName(i, strLayerName);
            pLevel->GetLayer(i)->SetName(strLayerName);
        }
        else
        {
            wstring strLayerName = CLevelMgr::GetInst()->GetLayerName(i);
            pLevel->GetLayer(i)->SetName(strLayerName);
        }
    }
    
    fclose(pFile);


    m_bFirstLoad = false;
    return pLevel;
}



CGameObject* CSaveLoadMgr::LoadGameObject(FILE* _File)
{
    // obj name
    CGameObject* pObject = new CGameObject;
    pObject->LoadFromFile(_File);

    int iLayerIdx = 0;
    fread(&iLayerIdx, sizeof(int), 1, _File);
    pObject->SetLayerIdx(iLayerIdx);

    // obj components
    bool bProgress = true;
    while (true)
    {
        // Component Type
        COMPONENT_TYPE type = COMPONENT_TYPE::END;
        fread(&type, sizeof(COMPONENT_TYPE), 1, _File);

        CComponent* pComponent = nullptr;

        switch (type)
        {
        case COMPONENT_TYPE::TRANSFORM:
            pComponent = new CTransform;
            break;

        case COMPONENT_TYPE::CAMERA:
            pComponent = new CCamera;
            break;

        case COMPONENT_TYPE::COLLIDER2D:
            pComponent = new CCollider2D;
            break;

        case COMPONENT_TYPE::ANIMATOR2D:
            pComponent = new CAnimator2D;
            break;

        case COMPONENT_TYPE::LIGHT2D:
            pComponent = new CLight2D;
            break;

        case COMPONENT_TYPE::MESHRENDER:
            pComponent = new CMeshRender;
            break;

        case COMPONENT_TYPE::TILEMAP:
            pComponent = new CTileMap;
            break;

        case COMPONENT_TYPE::PARTICLESYSTEM:
            pComponent = new CParticleSystem;
            break;
            
        case COMPONENT_TYPE::RIGIDBODY2D:
            pComponent = new CRigidbody2D;
            break;

        case COMPONENT_TYPE::SCRIPT:
            break;
        case COMPONENT_TYPE::COLLIDER3D:
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
            break;
        case COMPONENT_TYPE::LIGHT3D:
            break;
        case COMPONENT_TYPE::SKYBOX:
            break;
        case COMPONENT_TYPE::DECAL:
            break;
        case COMPONENT_TYPE::LANDSCAPE:
            break;

        case COMPONENT_TYPE::END:
            bProgress = false;
            break;
        }

        if (!bProgress)
            break;

        pComponent->LoadFromFile(_File);
        pObject->AddComponent(pComponent);
    }


    // script
    size_t iScriptCount = 0;

    // load script count
    fread(&iScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring strScriptName;
        // load script name
        LoadWStringFromFile(strScriptName, _File);

        // load script member
        CScript* pNewScript = CScriptMgr::GetScript(strScriptName);
        pNewScript->LoadFromFile(_File);

        // add component
        pObject->AddComponent(pNewScript);
    }


    // child
    size_t iChildCount = 0;

    // load child count
    fread(&iChildCount, sizeof(size_t), 1, _File);

    // load child member
    for (size_t i = 0; i < iChildCount; ++i)
    {
        CGameObject* pChild = LoadGameObject(_File);
        // add child
        pObject->AddChild(pChild);
    }

    return pObject;
}

