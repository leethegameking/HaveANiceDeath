#ifndef _STRUCT
#define _STRUCT

// test

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
    uint    iActive; // 0 Ready | 1 Play | 2 Dead
};

struct tAnimIdx
{
    int iAnimIdx;
    float fAcc;
    int iPadding[2];
};

struct tParticleShare
{
    uint iAliveCount;
    uint3 iPadding;
};

struct tParticleAnim
{
    float2 vLeftTop;
    float2 vSlice;
    float2 vOffset;
    float2 vFullSize;
    float fDuration;
    float fAcc;
    
    int iEnd;
    int iPadding;
};

struct tAnim2DInfo
{
    float2 vLeftTop;
    float2 vSlice;
    float2 vOffset;
    float2 vFullSize;

    int iAnim2DUse;
    int iPadding[3];
};




#endif
