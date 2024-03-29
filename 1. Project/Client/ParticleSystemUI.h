﻿#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class ComboBox;

class ParticleSystemUI :
    public ComponentUI
{
private:
    int                        m_iMaxCount;
    int                        m_iAliveCount;

    Vec4                        m_vStartScale;
    Vec4                        m_vEndScale;

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;

    Vec2                        m_vMinMaxSpeed;
    Vec2                        m_vMinMaxLifeTime;

    Vec2                        m_fSpawnRange;

    float                       m_Frequency;    // 초당 파티클 생성 수
    float                       m_fAccTime;     // 시간 누적

    int                         m_WorldSpawn;

    string                      m_TexKey;
    string                      m_CSKey;

    ComboBox*                   m_CSComboBox;

    ////--------------------------------------------

    Ptr<CMesh>      m_Mesh;
    Ptr<CMaterial>  m_Mtrl;
    bool            m_bDynamicMtrl;

public:
    void SetTexture(DWORD_PTR _texKey);

public:
    void init() override;
    void update();
    void render_update();

private:
    void MtrlBtn();
    void SetMaterial(DWORD_PTR _strMaterialKey);
    void SetMesh(DWORD_PTR _strMeshKey);

public:
    void SetCSKey(DWORD_PTR _CSKey);


public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

