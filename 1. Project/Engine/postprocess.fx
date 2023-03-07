#ifndef _POST_PROCESS
#define _POST_PROCESS

#include "register.fx"

Texture2D LightTex : register(t17);

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


//VS_OUT VS_PostProcess(VS_IN _in)
//{
//    VS_OUT output = (VS_OUT) 0.f;
    
//    output.vPosition = float4(_in.vPos.xy * 2.f, 0.5f, 1.f);
//    output.vUV = _in.vUV;
    
//    return output;
//}


//float4 PS_PostProcess(VS_OUT _in) : SV_Target
//{    
//    float4 vColor = (float4) 0.f;
        
//    vColor = g_RTCopyTex.Sample(g_sam_0, _in.vUV);    
    
//    vColor.rgb = (vColor.r + vColor.g + vColor.b) / 3.f;
    
//    return vColor;
//}

VS_OUT VS_PostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_PostProcess(VS_OUT _in) : SV_Target
{
    // 기존
    //float4 vColor = (float4) 0.f;
          
    //// VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    //float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    ////vUV.y += cos((_in.vUV.x - g_fAccTime * 0.5f) * 10.f * 3.141592f) * 0.01f;
    //float2 fAdd = float2(g_Noise.Sample(g_sam_0, _in.vUV + g_fAccTime * 0.2f).x
    //                     , g_Noise.Sample(g_sam_0, _in.vUV + float2(0.1f, 0.f) + g_fAccTime * 0.2f).x);
    //fAdd -= fAdd / 2.f;
    //vUV += fAdd * 0.05f;
    
    //vColor = g_RTCopyTex.Sample(g_sam_0, vUV);
    
    //vColor.r *= 1.5f;
    
    //return vColor;
    
    // 보라색 이펙트 추가용
    float4 vColor = (float4) 0.f;
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    vColor = float4(1.0f, 0.f, 1.0f, 0.f);
    
    for (int i = -10; i < 10; ++i)
    {
        for (int j = -10; j < 10; ++j)
        {
            float2 vPostion = _in.vPosition.xy + float2(i, j);
            float2 vNearUV = vPostion / g_vRenderResolution;
            float4 vNearColor = g_RTCopyTex.Sample(g_sam_0, vNearUV);
            if (vNearColor.r == 1.0f && vNearColor.g == 1.0f)
            {
                vColor.a += 0.0005f;
            }
        }

    }
    float4 vOutColor = g_RTCopyTex.Sample(g_sam_0, vUV);
    
    if (vOutColor.r == 1.0f && vOutColor.g == 1.0f && vOutColor.b == 1.0f)
    {
        if(vOutColor.a != 1.f)
        {
            vColor.a += vOutColor.a / 10.f;
            
            return vColor;
        }
        return float4(0.f, 0.f, 0.f, 0.f);
    }
    
    return vColor;
}

// Light Post Process
VS_OUT VS_LightPostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
    
    //VS_OUT output = (VS_OUT) 0.f;
    
    //output.vPosition = float4(_in.vPos.xy * 2.f, 0.5f, 1.f);
    //output.vUV = _in.vUV;
    
    //return output;
}


float4 PS_LightPostProcess(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    float4 vLight = (float4) 0.f;
    //float4 LightViewSpace = _in.vPosition;
    
    //LightViewSpace.x -= 800.f;
    //LightViewSpace.y -= 450.f;
    //// LightViewSpace = mul(float4(LightViewSpace.x - 800.f, LightViewSpace.y - 450.f, LightViewSpace.z, 1.f), g_matView);
    //// LightViewSpace += float4(500.f, 0.f, 0.f, 0.f);
    //LightViewSpace = mul(LightViewSpace, g_matWVP);
    
    //_in.vPosition.x;
    //-_in.vPosition.y;
    
    //float2 vUV = LightViewSpace.xy / g_vRenderResolution;
    ////float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    
    //tmpUV = mul(mul(tmpUV, g_matView), g_matProj);
    

    vLight = LightTex.Sample(g_sam_0, _in.vUV);

    
    vColor = g_RTCopyTex.Sample(g_sam_0, _in.vUV);
    
    
    // vColor.rgb = (vColor.r + vColor.g + vColor.b) / 3.f;
     vColor += vLight.xxxx / 2.f * vLight.y ;
    
    return vColor;
}







#endif

