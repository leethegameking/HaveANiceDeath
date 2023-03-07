#pragma once
#include "CComputeShader.h"
#include "Ptr.h"
#include "CTexture.h"

class CParticleSystem;
class CStructuredBuffer;

class CParticleUpdateShader :
    public CComputeShader
{
public:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_ParticleShare;
    CStructuredBuffer*  m_ParticleAnim;
    CStructuredBuffer*  m_ParticleAnimFrm;

    Ptr<CTexture>       m_LightTex;

    CParticleSystem*    m_Owner;


public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer);
    void SetParticleShareData(CStructuredBuffer* _Buffer){ assert(_Buffer); m_ParticleShare = _Buffer; }
    void SetParticleAnim(CStructuredBuffer* _Buffer);
    void SetParticleAnimFrm(CStructuredBuffer* _Buffer);

    void SetWorldSpawn(bool _bWorldSpawn) {m_Param.iArr[1] = (int)_bWorldSpawn; }
    void SetObjectWorldPos(Vec3 _vWorldPos) { m_Param.v4Arr[0] = _vWorldPos; }
    void SetMinMaxSpeed(Vec2 _vSpeed) { m_Param.v2Arr[0] = _vSpeed; }
    void SetMinMaxLifeTime(Vec2 _vLifeTime) { m_Param.v2Arr[1] = _vLifeTime; }
    void SetSpawnRange(Vec2 _Range) { m_Param.v2Arr[2] = _Range; }
    void SetFirstEntry(int _i) { m_Param.iArr[2] = _i; }

    void SetLightTex(wstring _texKey);

    void SetOwner(CParticleSystem* _pOwner) { m_Owner = _pOwner; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};

