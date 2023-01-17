#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>

class MeshRenderUI :
    public ComponentUI
{
private:
    Ptr<CMesh>      m_Mesh;
    Ptr<CMaterial>  m_Mtrl;
    TEX_PARAM       m_eSelectTexParam;
    bool            m_bDynamicMtrl;

public:
    virtual void update() override;
    virtual void render_update() override;

private:
    void MtrlBtn();
    void TextureBtn();
    

private:
    void SetMesh(DWORD_PTR _strMeshKey);
    void SetMaterial(DWORD_PTR _strMaterialKey);
    void SetDynMaterialTex(DWORD_PTR _strTextureKey);
    void SetTexture(DWORD_PTR _strTexKey);

private:
    virtual void Close() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

