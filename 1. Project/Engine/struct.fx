#ifndef _STRUCT
#define _STRUCT

struct tLightColor
{
    float4 vDiff; // 난반사 광
    float4 vSpec; // 반사광 계수
    float4 vEmb; // 환경광
};


// LightInfo
struct tLightInfo
{
    tLightColor color;	
    float4      vWorldPos;  // 광원 월드 위치
    float4      vWorldDir;  // 광원의 월드 방향
    int         iLightType; // 광원 타입
    float       fRadius;    // 광원 반경 (POINT, SPOT)
    float       fAngle;     // 광원 각도 (SPOT)
    int         pad;
};


struct tParticle
{
    float4  vRelativePos;
    float4  vDir;
		
    float   fMaxTime;
    float   fCurTime;
    float   fSpeed;
    uint    iActive;
};

struct tParticleShare
{
    uint iAliveCount;
    uint3 iPadding;
};





#endif
