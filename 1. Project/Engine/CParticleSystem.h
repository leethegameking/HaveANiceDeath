#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    UINT                        m_iMaxCount;
    UINT                        m_iAliveCount;

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

    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_ParticleShare;
    Ptr<CParticleUpdateShader>  m_UpdateCS;

    UINT                        m_RenderType;

    wstring                     CS_KEY;
    wstring                     TEX_KEY;
    wstring                     LIGHT_TEX_KEY;

public:
    UINT GetMaxCount() { return m_iMaxCount; }
    UINT GetAliveCount() { return m_iAliveCount; }

    Vec4 GetStartScale() { return m_vStartScale; }
    Vec4 GetEndScale() { return m_vEndScale; }

    Vec4 GetStartColor() { return m_vStartColor; }
    Vec4 GetEndColor() { return m_vEndColor; }

    Vec2 GetMinMaxSpeed() { return m_vMinMaxSpeed; }
    Vec2 GetMinMaxLife() { return m_vMinMaxLifeTime; }

    Vec2 GetSpawnRange() { return m_fSpawnRange; }

    float GetFrequency() { return m_Frequency; }
    float GetAccTime() { return m_fAccTime; }

    int GetWorldSpawn() { return m_WorldSpawn; }
    wstring GetTexKey() { return TEX_KEY; }

    void SetWorldSpawn(bool _bSet) { m_WorldSpawn = (int)_bSet; }
    void SetRenderType(UINT _iType) { m_RenderType = _iType; }
    void SetScale(Vec4 _fStart, Vec4 _fEnd) { m_vStartScale = _fStart; m_vEndScale = _fEnd; }
    void SetColor(Vec4 _fStart, Vec4 _fEnd) { m_vStartColor = _fStart; m_vEndColor = _fEnd; }
    void SetColorConvert(Vec4 _fStart, Vec4 _fEnd) 
    {
        m_vStartColor = _fStart / 255.f; m_vEndColor = _fEnd / 255.f;
        m_vStartColor.w = 1.f;           m_vEndColor.w = 1.f;
    }
    void SetLifeTime(Vec2 _fMinMax) { m_vMinMaxLifeTime = _fMinMax; }
    void SetAliveCount(UINT _count) { m_iAliveCount = _count; }
    void SetMaxCount(UINT _count) { m_iMaxCount = _count; }
    void SetSpeed(Vec2 _speed) { m_vMinMaxSpeed = _speed; }
    void SetSpawnRange(Vec2 _spawnRange) { m_fSpawnRange = _spawnRange; }
    void SetFrequency(float _frequency) { m_Frequency = _frequency; }
    void SetTexture(wstring _texKey);
    void SetLightTex(wstring _texKey);

    void ReduceLifeTime() { m_vMinMaxLifeTime -= 0.1f; }
    void ReduceAliveCount() { m_iAliveCount -= 1; }
    void ReduceSpeed() { m_vMinMaxSpeed -= 0.1f; }
    void ReduceFrequency() { m_Frequency -= 0.01f; }
    void ReduceSpawnRange() { m_fSpawnRange -= 0.01f; }

    void RaiseLifeTime() { m_vMinMaxLifeTime += 0.1f; }
    void RaiseAliveCount() { m_iAliveCount += 1; }
    void RaiseSpeed() { m_vMinMaxSpeed += 0.1f; }
    void RaiseFrequency() { m_Frequency += 0.01f; }
    void RaiseSpawnRange() { m_fSpawnRange += 0.01f; }

    Ptr<CParticleUpdateShader> GetUpdateCS() { return m_UpdateCS; }

    //void SetUpdateCS(wstring _CSkey){ m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(_CSkey).Get(); }

public:
    void ChangeBuffer();

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(wstring _CSkey);
    CParticleSystem(const CParticleSystem& _clone);
    ~CParticleSystem();
};

