#ifndef _RAINPARTICLEUPDATE
#define _RAINPARTICLEUPDATE

#include "register.fx"
#include "func.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleShare> ParticleShare : register(u1);


#define PARTICLE_MAX    g_int_0
#define Particle        ParticleBuffer[_id.x]
#define ShareData       ParticleShare[0]

#define SpawnRange      g_vec2_2
#define IsWorldSpawn    g_int_1
#define ObjectWorldPos  g_vec4_0
#define MinSpeed        g_vec2_0.x
#define MaxSpeed        g_vec2_0.y
#define MinLife         g_vec2_1.x
#define MaxLife         g_vec2_1.y



[numthreads(128, 1, 1)]
void CS_ParticleUpdate(uint3 _id : SV_DispatchThreadID)
{
    if ((uint) PARTICLE_MAX <= _id.x)
        return;
            
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
            // 랜덤값으로 위치와 방향을 설정한다.
            // 샘플링을 시도할 UV 를 계산한다.
            float4 vRandom = (float4) 0.f;
            
            float2 vUV = float2((float) _id.x / PARTICLE_MAX, 0.5f);
            vUV.x += g_fAccTime;
            vUV.y += sin((vUV.x + g_fAccTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float4(GaussianSample(vUV + float2(0.f, 0.f)).x, GaussianSample(vUV + float2(0.1f, 0.f)).x, GaussianSample(vUV + float2(0.2f, 0.f)).x, GaussianSample(vUV + float2(0.3f, 0.f)).x);
           
            vRandom.x = 87.f * vRandom.x;
            //vRandom.x = vRandom.x - floor(vRandom.x);
            vRandom.x = frac(vRandom.x);
            
            vRandom.y = 87.f * vRandom.y;
            //vRandom.x = vRandom.x - floor(vRandom.x);
            vRandom.y = frac(vRandom.y);
            
            // 사각형범위로 스폰
            Particle.vRelativePos.x = vRandom.x * SpawnRange.x - 1200.f;
            Particle.vRelativePos.y = 450.f;
            
            
            
            // 원형 범위로 스폰
            //float fTheta = vRandom.x * 3.141592f * 2.f;
            //Particle.vRelativePos.xy = float2(cos(fTheta), sin(fTheta)) * vRandom.y * SpawnRange.x;
            //Particle.vRelativePos.z = 0.f;
            //Particle.vRelativePos.xyz = float3(0.f, 0.f, 0.f);
            
            // 파티클 방향
            // Particle.vDir.xy = normalize(float2(Particle.vRelativePos.xy));
            Particle.vDir.xy = normalize(float2(-0.5f, -1.f + vRandom.x / 4.f - 0.5f));

            
            // 생성될 때 물체의 위치에 간섭 -> render에서는 물체를 따라다니게 함.
            if (IsWorldSpawn)
            {
               Particle.vRelativePos.xyz += ObjectWorldPos.xyz;
            }
            
            // 파티클 속력
            Particle.fSpeed = vRandom.z * (MaxSpeed - MinSpeed) + MinSpeed;
            
            // 파티클 Life
            Particle.fCurTime = 0.f;
            Particle.fMaxTime = MaxLife; //vRandom.w * (MaxLife - MinLife) + MinLife;
        }
    }
    // 활성화 된 파티클을 업데이트
    else
    {
        Particle.fCurTime += g_fDT;
        if (Particle.fMaxTime < Particle.fCurTime)
        {
            Particle.iActive = 0;
        }
        else
        {
            Particle.vRelativePos.xy += Particle.vDir.xy * Particle.fSpeed * g_fDT;
        }
    }
}

#endif
