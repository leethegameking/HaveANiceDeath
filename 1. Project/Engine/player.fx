#ifndef _PLAYER
#define _PLAYER

#include "register.fx"
#include "func.fx"

#define ALPHA_RATIO g_float_0
#define BOOL_CLOSE  g_int_0
#define CLOSE_SPEED g_float_1
#define CLOSED_RATIO g_float_2

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


VTX_OUT VS_Player(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Player_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (BOOL_CLOSE)
    {
        //if ((g_v3Scale.x - CLOSE_SPEED * g_fDT) / g_v3Scale - CLOSED_RATIO >= _in.vUV.x || _in.vUV.x >= CLOSED_RATIO + CLOSE_SPEED * g_fDT / g_v3Scale.x)
        if (CLOSED_RATIO / 2.f >= _in.vUV.x || _in.vUV.x >= 1.f - CLOSED_RATIO / 2.f)
        {
            discard;
        }
    }

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);


        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
        
        vOutColor = g_Atals.Sample(g_sam_0, vUV);
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }

    

    
    vOutColor.a *= ALPHA_RATIO;

    return vOutColor;
}



#endif


