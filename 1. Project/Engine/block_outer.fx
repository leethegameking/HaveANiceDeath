#ifndef _BLOCK_OUTER
#define _BLOCK_OUTER

#include "register.fx"
#include "func.fx"


#define ObjScale g_v3Scale

#define TextureIdx g_int_0

// Texture 
#define CornerInside    0 // g_tex_0
#define CornerOutside   1 // g_tex_1
#define CornerOrnements 2 // g_tex_2
#define BlockBorder     3 // g_tex_3

#define CornerInsideTex      g_tex_0
#define CornerOutsideTex     g_tex_1
#define CornerOrnementsTex   g_tex_2
#define BlockBorderTex       g_tex_3






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


VTX_OUT VS_Block_Outer(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Block_Outer(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    
    //Texture2D sampleTex;
    switch (TextureIdx)
    {
        case CornerInside:
            vOutColor = CornerInsideTex.Sample(g_sam_0, _in.vUV);
            break;
        
        case CornerOutside:
            vOutColor = CornerOutsideTex.Sample(g_sam_0, _in.vUV);
            break;
        
        case CornerOrnements:
            vOutColor = CornerOrnementsTex.Sample(g_sam_0, _in.vUV);
            break;
        
        case BlockBorder:
            vOutColor = BlockBorderTex.Sample(g_sam_0, _in.vUV);
            break;
        
        default:
            break;
    }
   
    return vOutColor;
}

float4 PS_Block_Outer_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    

    
    
    //Texture2D sampleTex;
    switch (TextureIdx)
    {
        case CornerInside:
            vOutColor = CornerInsideTex.Sample(g_sam_0, _in.vUV);
            break;
        
        case CornerOutside:
            vOutColor = CornerOutsideTex.Sample(g_sam_0, _in.vUV);
            break;
        
        case CornerOrnements:
            vOutColor = CornerOrnementsTex.Sample(g_sam_0, _in.vUV);
            break;
        
        // 크기 줄여줌
        case BlockBorder:
        {
            float2 fBlockScale = float2(500.f, 50.f);
            float2 fBlockRatio = ObjScale.xy / fBlockScale;
            float2 vUV = _in.vUV * fBlockRatio;
    
            //if (fBlockRatio.x >= 1.f)
            //{
            //    fBlockRatio.x = 1.f;
            //}
            if (fBlockRatio.y >= 1.f)
            {
                fBlockRatio.y = 1.f;
            }
            
            vOutColor = BlockBorderTex.Sample(g_sam_0, vUV);
        }
            break;
        
        default:
            break;
    }
    
    // 광원 처리
    tLightColor color = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }
    
    color.vDiff.a = 1.f;
    vOutColor *= color.vDiff;
   
    return vOutColor;
}



#endif


