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
    //output.vPosition.x = _in.vPos.x * (RelativeScale.x / g_vAtlasSize.x);
    //output.vPosition.y = _in.vPos.y * (RelativeScale.y / g_vAtlasSize.y);
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

//PS_OUT PS_Std2D_Rescaling(VS_OUT _in) : SV_Target
//{
//    // 1. sampling
//    PS_OUT output;
//    float4 vColor;
    
//    if (g_UseAnim2D)
//    {
//        // 2. Rescaling
//        float ResizeX = RelativeScale.x / g_vAtlasSize.x;
//        float ResizeY = RelativeScale.y / g_vAtlasSize.y;
//        float2 vLeftTop;
//        float2 vCutSize;
//        float2 vBackgroundSize;
//        vLeftTop.x = g_vLeftTop.x * ResizeX;
//        vLeftTop.y = g_vLeftTop.y * ResizeY;
//        vCutSize.x = vBackgroundSize.x = g_vBackgroundSize.x * ResizeX;
//        vCutSize.y = vBackgroundSize.y = g_vBackgroundSize.y * ResizeY;
        
//        float2 vBackgroundLeftTop = vLeftTop + (vCutSize / 2.f) - (vBackgroundSize / 2.f);
//        float2 vUV = vBackgroundLeftTop + _in.vUV * vBackgroundSize; //UV는 한 프레임의 크기를 대상으로 하므로, 프레임의 크기만큼 곱해줘야 함
        
//        // 가져오려는 이미지를 벗어나면 그리지 않음
//        // 즉, 부족한 부분에 대해서만 더 그림
//        if (vUV.x < g_vLeftTop.x || vUV.x > g_vLeftTop.x + g_vCutSize.x
//            || vUV.y < g_vLeftTop.y || vUV.y > g_vLeftTop.y + g_vCutSize.y)
//            discard;
//        else
//            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
//    }
//    else
//    {
//        if (g_btex_0)
//        {
//            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
//        }
//    }
    
//    // 2. lighting
//    tLightColor LightColor = (tLightColor) 0.f;
    
//    for (int i = 0; i < g_Light2DCount; ++i)
//        CalLight2D(_in.vWorldPos, i, LightColor);
    
//    vColor.rgb *= LightColor.vColor.rgb + LightColor.vAmbient.rgb;
    
//    // 3. cut masked
//    if (vColor.a == 0.f)
//        discard;
    
//    // render target 0
//    output.RenderTarget = vColor;
//    output.GlowTarget = float4(0.f, 0.f, 0.f, 1.f);
//    return output;
//}

#endif
