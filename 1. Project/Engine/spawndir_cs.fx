#ifndef _SPAWNDIR_CS
#define _SPAWNDIR_CS

#include "register.fx"
#include "func.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleShare> ParticleShare : register(u1);
RWStructuredBuffer<tParticleAnim> ParticleAnim : register(u2);
RWStructuredBuffer<tAnimIdx> ParticleAnimIdx : register(u3);


#define PARTICLE_MAX    g_int_0
#define Particle        ParticleBuffer[_id.x]
#define ShareData       ParticleShare[0]

#define AnimFrm         ParticleAnim
#define AnimIdx         ParticleAnimIdx[_id.x]

#define SpawnRange      g_vec2_2
#define IsWorldSpawn    g_int_1
#define ObjectWorldPos  g_vec4_0
#define MinSpeed        g_vec2_0.x
#define MaxSpeed        g_vec2_0.y
#define MinLife         g_vec2_1.x
#define MaxLife         g_vec2_1.y

#define LEFT            1.f
#define RIGHT           -1.f

#define RANGE_X         30.f

#define FirstState      g_int_2


[numthreads(128, 1, 1)]
void CS_SpawnDirUpdate(uint3 _id : SV_DispatchThreadID)
{
    if ((uint) PARTICLE_MAX <= _id.x)
        return;

    if (FirstState)
        Particle.iActive = 0;
    
    if (0 == Particle.iActive)
    {
        while (0 < ShareData.iAliveCount)
        {
            int iOriginValue = ShareData.iAliveCount;
            int iExchange = iOriginValue - 1;
        
            //InterlockedExchange(ShareData.iAliveCount, iExchange, iExchange);
            InterlockedCompareExchange(ShareData.iAliveCount, iOriginValue, iExchange, iExchange);
            
            if (iOriginValue == iExchange)
            {
                Particle.iActive = 1;
                break;
            }
        }
        
        if (Particle.iActive)
        {
            Particle.fCurTime = 0.f;
            // ���������� ��ġ�� ������ �����Ѵ�.
            // ���ø��� �õ��� UV �� ����Ѵ�.
            float4 vRandom = (float4) 0.f;
            
            float2 vUV = float2((float) _id.x / PARTICLE_MAX, 0.5f);
            vUV.x += g_fAccTime;
            vUV.y += sin((vUV.x + g_fAccTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float4(GaussianSample(vUV + float2(0.f, 0.f)).x, GaussianSample(vUV + float2(0.1f, 0.f)).x, GaussianSample(vUV + float2(0.2f, 0.f)).x, GaussianSample(vUV + float2(0.3f, 0.f)).x);
           
            vRandom.x = 87.l * vRandom.x;
            vRandom.x = frac(vRandom.x);
            
            vRandom.y = 87.l * vRandom.y;
            vRandom.y = frac(vRandom.y);
            
            vRandom.z = 87.l * vRandom.z;
            vRandom.z = frac(vRandom.z);
            
            
            // change x spawn
            float fTheta = vRandom.x * 3.141592f * 2.f;
            Particle.vRelativePos.xy = float2(cos(fTheta), sin(fTheta)) * SpawnRange.xy * vRandom.z;
            Particle.vRelativePos.z = 0.f;
            
            // ��ƼŬ ���� (0,0) �� ��� ����.
            Particle.vDir.xy = normalize(float2(Particle.vRelativePos.x, Particle.vRelativePos.y));
            
            // ������ �� ��ü�� ��ġ�� ���� -> render������ ��ü�� ����ٴϰ� ��.
            if (IsWorldSpawn)
            {
               Particle.vRelativePos.xyz += ObjectWorldPos.xyz;
            }
            
            // Particle speed
            Particle.fSpeed = vRandom.y * (MaxSpeed - MinSpeed) + MinSpeed;
            
            // ��ƼŬ Life
            Particle.fCurTime = 0.f;
            Particle.fMaxTime = vRandom.z * (MaxLife - MinLife) + MinLife;
            
            //================== anim ====================
            
            AnimIdx.iAnimIdx = 0;
            AnimIdx.fAcc = 0.f;
        }
    }
    
    // Ȱ��ȭ �� ��ƼŬ�� ������Ʈ
    else
    {
        Particle.fCurTime += g_fDT;
        if (Particle.fMaxTime < Particle.fCurTime)
        {
            Particle.iActive = 2;
        }
        else if (Particle.iActive == 1)
        {
            Particle.vRelativePos.xy += Particle.vDir.xy * Particle.fSpeed * g_fDT;

            AnimIdx.fAcc += g_fDT;
            if (AnimFrm[AnimIdx.iAnimIdx].fDuration < AnimIdx.fAcc)
            {
                AnimIdx.fAcc = 0;
                AnimIdx.iAnimIdx += 1.f;
            }
        }
    }
}

#endif