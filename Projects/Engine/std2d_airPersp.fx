#ifndef _STD2D_AIRPERSP
#define _STD2D_AIRPERSP

#include "value.fx"
#include "func.fx"

#define AirPerspUnable g_int_0
#define AirColor g_vec4_0
#define ObjDepth g_float_0

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

struct PS_OUT
{
    float4 RenderTarget : SV_Target0;
    float4 GlowTarget : SV_Target1;
};

PS_OUT PS_Std2D_AirPersp(VS_OUT _in) : SV_Target
{
    // 1. sampling
    PS_OUT output;
    float4 vColor;
    
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
        float2 vUV = vBackgroundLeftTop + _in.vUV * g_vBackgroundSize;
        
        if (vUV.x < g_vLeftTop.x || vUV.x > g_vLeftTop.x + g_vCutSize.x
            || vUV.y < g_vLeftTop.y || vUV.y > g_vLeftTop.y + g_vCutSize.y)
            discard;
        else
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        
    //    // alpha blending (magenta background delete)
    //    float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f); // saturate: 0~1 넘지 않게 보정
        
    //    if (fAlpha == 0.f)
    //    {
    //        discard;
    //    }
    //
        }
    
    // Air Perspective
    if (AirPerspUnable && ObjDepth != 0.f)
    {
        float alpha = ObjDepth / 2000.f;
        float4 airColor = AirColor;
        vColor.rgb = vColor.rgb * (1 - alpha) + airColor.rgb * alpha * AirColor.a;
    }
    
    // 2. lighting
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
        CalLight2D(_in.vWorldPos, i, LightColor);
    
    vColor.rgb *= LightColor.vColor.rgb + LightColor.vAmbient.rgb;
    
    // 3. cut masked
    if (vColor.a == 0.f)
        discard;
    
    // render target 0
    output.RenderTarget = vColor;
    return output;
}

#endif