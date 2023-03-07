#ifndef _DISTORDER
#define _DISTORDER

#include "register.fx"
#include "func.fx"

#define SAMPLER_TYPE g_int_0

#define MUL_OR_ADD g_int_1

#define EXCLUDE_LIGHT g_int_2

#define RED     g_float_0
#define GREEN   g_float_1
#define BLUE    g_float_2
#define ALPHA   g_float_3

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


VTX_OUT VS_Dist_Order(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Dist_Order_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);

        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
        
        if(SAMPLER_TYPE == 0)
        {
            vOutColor = g_Atals.Sample(g_sam_0, vUV);
        }
        else
        {
            vOutColor = g_Atals.Sample(g_sam_1, vUV);
        }
    }
    else
    {
        if (SAMPLER_TYPE == 0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
    }
    
    if(MUL_OR_ADD == 0)
    {
        vOutColor.r *= 1.f + RED;
        vOutColor.g *= 1.f + GREEN;
        vOutColor.b *= 1.f + BLUE;
        vOutColor.a *= 1.f + ALPHA;
    }
    else
    {
        vOutColor.r += RED;
        vOutColor.g += GREEN;
        vOutColor.b += BLUE;
        vOutColor.a += ALPHA;
    }

    return vOutColor;
}

float4 PS_Dist_Order_Light_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);

        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
        
        if (SAMPLER_TYPE == 0)
        {
            vOutColor = g_Atals.Sample(g_sam_0, vUV);
        }
        else
        {
            vOutColor = g_Atals.Sample(g_sam_1, vUV);
        }
    }
    else
    {
        if (SAMPLER_TYPE == 0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
    }
    
    if (MUL_OR_ADD == 0)
    {
        vOutColor.r *= 1.f + RED;
        vOutColor.g *= 1.f + GREEN;
        vOutColor.b *= 1.f + BLUE;
        vOutColor.a *= 1.f + ALPHA;
    }
    else
    {
        vOutColor.r += RED;
        vOutColor.g += GREEN;
        vOutColor.b += BLUE;
        vOutColor.a += ALPHA;
    }
    
    if(!EXCLUDE_LIGHT)
    {
    // ±¤¿ø Ã³¸®
        tLightColor color = (tLightColor) 0.f;
    
        for (int i = 0; i < g_iLight2DCount; ++i)
        {
            CalcLight2D(color, _in.vWorldPos, i);
        }
        
        color.vDiff.a = 1.f;
        vOutColor *= color.vDiff;
    }

    return vOutColor;
}


#endif


