#ifndef _STD2D_RESCALING
#define _STD2D_RESCALING

#include "value.fx"
#include "func.fx"

#define RelativeScale g_vec2_0

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
    float3 vWorldPos : POSITION;
};

struct PS_OUT
{
    float4 RenderTarget : SV_Target0;
    float4 GlowTarget : SV_Target1;
};

VS_OUT VS_Std2D_Rescaling(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // World로 이동
    output.vPosition.xyz = _in.vPos;
    output.vPosition.xy *= RelativeScale / (g_vCutSize * g_vAtlasSize);
    output.vPosition.w = 1.f;
    output.vPosition = mul(output.vPosition, g_matWorld);
    
    // Offset 가져오기
    float2 vOffset = { g_vOffset.x * g_vAtlasSize.x, g_vOffset.y * g_vAtlasSize.y };
    vOffset *= RelativeScale / (g_vCutSize * g_vAtlasSize);
    
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
    
    // View Frustum Culling
    if (output.vPosition.x > g_RenderResolution.x && output.vPosition.y > g_RenderResolution.y)
    {
        output.vPosition = float4(0, 0, 0, 0); // 유효하지 않은 위치를 반환 (이후 RS, PS에서 처리 X)
    }
    
    output.vColor = _in.vColor;
    
    return output;
}

#endif
