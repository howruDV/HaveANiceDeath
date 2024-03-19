#ifndef _UI
#define _UI

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_UI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_UI(VS_OUT _in) : SV_Target
{
    // sampling
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        // Background
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (_in.vUV * g_vBackgroundSize); //UV는 한 프레임의 크기를 대상으로 함
        
        // 가져오려는 이미지를 벗어나면 그리지 않음
        // 즉, 부족한 부분에 대해서만 더 그림
        if (vUV.x < g_vLeftTop.x || vUV.x > g_vLeftTop.x + g_vCutSize.x
            || vUV.y < g_vLeftTop.y || vUV.y > g_vLeftTop.y + g_vCutSize.y)
            discard;
        vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
    }
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    
    return vColor;
};
#endif