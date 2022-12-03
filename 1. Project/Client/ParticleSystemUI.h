#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

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

    float                       m_Frequency;    // �ʴ� ��ƼŬ ���� ��
    float                       m_fAccTime;     // �ð� ����

    int                         m_WorldSpawn;

    string                      m_TexKey;

public:
    void SetTexture(DWORD_PTR _texKey);

public:
    void update();
    void render_update();


public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

