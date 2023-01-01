#ifndef _BLOCK_INNER
#define _BLOCK_INNER

#include "register.fx"
#include "func.fx"


#define ObjScale        g_v3Scale
#define TextureIdx      g_int_0
#define BlockFillTex    g_tex_0



// HLSL 로 VertexShader 작성하기
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_Block_Inner(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Block_Inner(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
   
    float2 fBlockScale = float2(1024.f, 1024.f);
    float2 fBlockRatio = ObjScale.xy / fBlockScale;
    float2 vUV = _in.vUV * fBlockRatio;
            
    vOutColor = BlockFillTex.Sample(g_sam_0, vUV);
    vOutColor.xyz *= 0.4f;
    
    return vOutColor;
}

float4 PS_Block_Inner_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
   
    float2 fBlockScale = float2(1024.f, 1024.f);
    float2 fBlockRatio = ObjScale.xy / fBlockScale;
    float2 vUV = _in.vUV * fBlockRatio;
    
    vOutColor = BlockFillTex.Sample(g_sam_0, vUV);

   
    return vOutColor;
}


#endif


