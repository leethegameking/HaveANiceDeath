#ifndef _GHOSTDEATH_CS
#define _GHOSTDEATH_CS

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
void CS_GhostDeathUpdate(uint3 _id : SV_DispatchThreadID)
{
    if ((uint) PARTICLE_MAX <= _id.x)
        return;

    if(FirstState)
    {
        Particle.iActive = 0;
    }
    
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
            // 랜덤값으로 위치와 방향을 설정한다.
            // 샘플링을 시도할 UV 를 계산한다.
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
            //float fTheta = vRandom.x * 3.141592f * 2.f;
            //Particle.vRelativePos.xy = float2(cos(fTheta), sin(fTheta)) * SpawnRange.xy * vRandom.y;
            //Particle.vRelativePos.z = 0.f;
            Particle.vRelativePos.xyz = (float3) 0.f;
            
            // 파티클 방향 (0,0) 일 경우 오류.
            Particle.vDir.xy = normalize(float2(vRandom.x - 0.5f, vRandom.y - 0.5f));
            
            // 생성될 때 물체의 위치에 간섭 -> render에서는 물체를 따라다니게 함.
            if (IsWorldSpawn)
            {
               Particle.vRelativePos.xyz += ObjectWorldPos.xyz;
            }
            
            // Particle speed
            Particle.fSpeed = vRandom.y * (MaxSpeed - MinSpeed) + MinSpeed;
            
            // 파티클 Life
            Particle.fCurTime = 0.f;
            Particle.fMaxTime = vRandom.z * (MaxLife - MinLife) + MinLife;
            
            //================== anim ====================
            
            AnimIdx.iAnimIdx = 0;
            AnimIdx.fAcc = 0.f;
        }
    }
    
    // 활성화 된 파티클을 업데이트
    else
    {
        Particle.fCurTime += g_fDT;
        if (Particle.fMaxTime < Particle.fCurTime)
        {
            Particle.iActive = 2;
        }
        else if (Particle.iActive == 1)
        {
            float fSpeed = Particle.fSpeed * (1.f - Particle.fCurTime / Particle.fMaxTime);
            Particle.vRelativePos.xy += Particle.vDir.xy * fSpeed * g_fDT;

            AnimIdx.fAcc += g_fDT;
            if (AnimFrm[AnimIdx.iAnimIdx].fDuration < AnimIdx.fAcc)
            {
                AnimIdx.fAcc = 0.f;
                AnimIdx.iAnimIdx += 1;
            }
        }
    }
}

#endif
