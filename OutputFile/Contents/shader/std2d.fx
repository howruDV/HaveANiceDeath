#ifndef _STD2D
#define _STD2D

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
    float3 vWorldPos : Position;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // World로 이동
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // Offset 가져오기
    float2 vOffset = { g_vOffset.x * g_vAtlasSize.x, g_vOffset.y * g_vAtlasSize.y };
    
    // Flip 적용
    output.vUV = _in.vUV;
    if (g_UseAnim2D)
    {
        if (g_FlipAnimXY & (1 << 1))
        {
            output.vUV.x = (1.f - output.vUV.x);
            vOffset.x *= -1;
        }
        
        if (g_FlipAnimXY & (1 << 0))
        {
            output.vUV.y = (1.f - output.vUV.y);
            vOffset.y *= -1;
        }
    }
    
    // World 저장
    output.vPosition -= float4(vOffset.x, vOffset.y, 0.f, 0.f);
    output.vWorldPos = output.vPosition.xyz;
    
    // View, Proj 행렬 
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vColor = _in.vColor;
    //output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // 1. sampling
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        // Background
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
        // vBackgroundLeftTop -= g_vOffset;
        
        // Flip
        float2 vUV = vBackgroundLeftTop + _in.vUV * g_vBackgroundSize; //UV는 한 프레임의 크기를 대상으로 하므로, 프레임의 크기만큼 곱해줘야 함
        
        // 가져오려는 이미지를 벗어나면 그리지 않음
        // 즉, 부족한 부분에 대해서만 더 그림
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
            
            // alpha blending (magenta background delete)
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f); // saturate: 0~1 넘지 않게 보정
            if (fAlpha < 0.1f)
            {
                // 픽셀 쉐이더 중간에 폐기
                // clip(-1);
                discard;
            }
        }
    }
    
    // 3. cut masked
    if (vColor.a == 0.f)
        discard;
    
    // 2. lighting
    tLightColor LightColor = (tLightColor) 0.f;
    for (int i = 0; i < g_Light2DCount; ++i)
        CalLight2D(_in.vWorldPos, i, LightColor);
    vColor.rgb *= LightColor.vColor.rgb + LightColor.vAmbient.rgb;
    
    // ex. effect - paper burn
    //float x = g_NoiseTex.Sample(g_sam_0, _in.vUV).x;
    //if (0 > x - g_float_1)
    //{
    //    discard;
    //}
    
    return vColor;
}

// Effect Shader
float4 PS_Std2D_Effect(VS_OUT _in) : SV_Target
{
    // 1. sampling
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        // Background
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (_in.vUV * g_vBackgroundSize);
        
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
            
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f); // saturate: 0~1 넘지 않게 보정
            if (fAlpha < 0.1f)
                discard;
        }
    }
    
    // 2. lighting
    tLightColor LightColor = (tLightColor) 0.f;
    for (int i = 0; i < g_Light2DCount; ++i)
        CalLight2D(_in.vWorldPos, i, LightColor);
    
    vColor.rgb *= LightColor.vColor.rgb + LightColor.vAmbient.rgb;
    return vColor;
}

#endif
