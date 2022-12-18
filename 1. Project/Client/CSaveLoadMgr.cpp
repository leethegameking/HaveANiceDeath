#include "pch.h"
#include "CSaveLoadMgr.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/GlobalComponent.h>

CSaveLoadMgr::CSaveLoadMgr()
{
}

CSaveLoadMgr::~CSaveLoadMgr()
{
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


    fclose(pFile);
}

void CSaveLoadMgr::SaveGameObject(CGameObject* _Object, FILE* _File)
{
    // obj name
    _Object->SaveToFile(_File);

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

    // child obj
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

    fclose(pFile);


    return pLevel;
}

CGameObject* CSaveLoadMgr::LoadGameObject(FILE* _File)
{
    // obj name
    CGameObject* pObject = new CGameObject;
    pObject->LoadFromFile(_File);

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

    // 자식 오브젝트

    return pObject;
}

