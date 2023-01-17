#ifndef _UI
#define _UI

#include "register.fx"
#include "func.fx"

// HP - UI
#define HP_RATIO g_float_0
#define INCLINATION g_float_1
#define TOP_DOT_DIST g_int_0

// PU_UI_ALPHA
#define TEX_SELECTION g_int_0

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


VTX_OUT VS_UI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_UI_Alpha(VTX_OUT _in) : SV_Target
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
        
        vOutColor = g_Atals.Sample(g_sam_0, vUV);
    }
    else
    {
        if(TEX_SELECTION == 0)
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else if(TEX_SELECTION == 1)
            vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    }
    
    return vOutColor;
}

float4 PS_Boss_HP_Bar_Alpha(VTX_OUT _in) : SV_Target
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
        
        vOutColor = g_Atals.Sample(g_sam_0, vUV);
    }
    else
    {
        if(HP_RATIO < _in.vUV.x)
            discard;
        
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    return vOutColor;
}

float4 PS_Player_HP_Bar_Alpha(VTX_OUT _in) : SV_Target
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
        
        vOutColor = g_Atals.Sample(g_sam_0, vUV);
    }
    else
    {
        // Dot located in Top Line UV
        float2 vTopDot = float2(HP_RATIO * (g_v3Scale.x - (float)TOP_DOT_DIST) / g_v3Scale.x, 0.f);
        
        // y = Inclination * (x - vTopDot)
        // y - Inclination * (x - vTopDot) < 0  -> dicard
        float eval = _in.vUV.y - INCLINATION * (_in.vUV.x - vTopDot);
        if( eval  < 0.f )
        {
            discard;
        }
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    return vOutColor;
}

#endif


