#ifndef _LIGHT_WRAP
#define _LIGHT_WRAP

#include "register.fx"



#define LightWrapTex g_tex_0 
// ====================
// PostProcess Shader
// mesh : RectMesh
// BS_TYPE : Default
// DS_TYPE : NoTest NoWrite 
// ====================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_LightWrap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_LightWrap(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = LightWrapTex.Sample(g_sam_0, _in.vUV);
    
    vColor = vColor.xxxx / 2.f * vColor.y;
    
    return vColor;
}







#endif