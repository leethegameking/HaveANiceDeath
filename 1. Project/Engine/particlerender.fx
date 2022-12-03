#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "register.fx"
#include "func.fx"


// ======================
// Particle Render Shader
// RS_TYPE : CULL_NONE
// BS_TYPE : ALPHABLEND
// DS_TYPE : NO_WRITE
// DOMAIN : Transparent
StructuredBuffer<tParticle> ParticleBuffer : register(t16);
#define Particle ParticleBuffer[_in[0].iInstance]
#define ParticleIndex   g_int_0
#define IsWorldSpawn    g_int_1

#define StartScale      g_vec4_0
#define EndScale        g_vec4_1
#define StartColor      g_vec4_2
#define EndColor        g_vec4_3
// ======================

//=======================
#define RenderType g_int_2

#define SPARK 2
//=======================

#define PI 3.141592


struct VS_IN
{
    float3 vPos : POSITION;    
    uint iInstance  : SV_InstanceID;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;    
    uint iInstance : SV_InstanceID;
};



VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vLocalPos = _in.vPos;
    output.iInstance = _in.iInstance;
        
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstance : SV_InstanceID;
};


[maxvertexcount(6)]
void GS_ParticleRender(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f};
    
    if (0 == ParticleBuffer[_in[0].iInstance].iActive)
        return;
   
    // 수명 비율
    float fRatio = ParticleBuffer[_in[0].iInstance].fCurTime / ParticleBuffer[_in[0].iInstance].fMaxTime;
    float3 vScale = lerp(StartScale.xyz, EndScale.xyz, fRatio);
        
    float3 vWorldPos = Particle.vRelativePos.xyz;
    
    // rectmesh를 돌려주고 뷰포즈에서 뺀다.
    
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    float3x3 RotMatrix = RotZMatrix(float2(Particle.vDir.x, Particle.vDir.y));
    
    float3 vNewViewPos[4] =
    {
        vViewPos.xyz - mul(float3(-0.5f, 0.5f, 0.f)  * vScale, RotMatrix),
        vViewPos.xyz - mul(float3(0.5f, 0.5f, 0.f)   * vScale, RotMatrix),
        vViewPos.xyz - mul(float3(0.5f, -0.5f, 0.f)  * vScale, RotMatrix),
        vViewPos.xyz - mul(float3(-0.5f, -0.5f, 0.f) * vScale, RotMatrix)
    }; 
    
    if (0 == IsWorldSpawn)
    {
        for (int i = 0; i < 4; ++i)
        {
            vNewViewPos[i].xyz += g_matWorld._41_42_43;
        }
    }
    
    //if (RenderType == 2)
    //{
    //    NewPos[0] = vViewPos - float3(-0.5f, 0.15f, 0.f) * vScale;
    //    NewPos[1] = vViewPos - float3(0.5f, 0.15f, 0.f) * vScale;
    //    NewPos[2] = vViewPos - float3(0.5f, -0.15f, 0.f) * vScale;
    //    NewPos[3] = vViewPos - float3(-0.5f, -0.15f, 0.f) * vScale;
    //}
    //else
    //{
    //    NewPos[0] = vViewPos - float3(-0.5f, 0.5f, 0.f) * vScale;
    //    NewPos[1] = vViewPos - float3(0.5f, 0.5f, 0.f) * vScale;
    //    NewPos[2] = vViewPos - float3(0.5f, -0.5f, 0.f) * vScale;
    //    NewPos[3] = vViewPos - float3(-0.5f, -0.5f, 0.f) * vScale;
    //}
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(vNewViewPos[i], 1.f), g_matProj);
        output[i].iInstance = _in[0].iInstance;
    }
    
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    //output[0].vUV = float2(1.f, 0.f);
    //output[1].vUV = float2(0.f, 0.f);
    //output[2].vUV = float2(0.f, 1.f);
    //output[3].vUV = float2(1.f, 1.f);
       
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
}




float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);    
    
    //if(RenderType == SPARK)
    //{
    //    float fRatio = ParticleBuffer[_in.iInstance].fCurTime / ParticleBuffer[_in.iInstance].fMaxTime;
    //    vColor.rgb *= lerp(StartColor, EndColor, fRatio).rgb;
    //}
    float fRatio = ParticleBuffer[_in.iInstance].fCurTime / ParticleBuffer[_in.iInstance].fMaxTime;
    vColor.rgb *= lerp(StartColor, EndColor, fRatio).rgb;
    // vColor.rgb = lerp(StartColor, EndColor, fRatio).rgb;
    
    return vColor;
}









#endif