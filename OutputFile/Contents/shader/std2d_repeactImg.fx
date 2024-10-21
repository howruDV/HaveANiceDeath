#ifndef _STD2D_REPEATIMG
#define _STD2D_REPEATIMG

#include "value.fx"
#include "func.fx"

#define RepeatX g_int_1
#define RepeatY g_int_2

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

VS_OUT VS_Std2D_RepeactImg(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // World로 이동
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // Offset 가져오기
    float2 vOffset = { g_vOffset.x * g_vAtlasSize.x, g_vOffset.y * g_vAtlasSize.y };
    
    // UV 조작(반복)
    output.vUV = _in.vUV;
    output.vUV.x *= RepeatX;
    output.vUV.y *= RepeatY;
    
    // World 저장
    output.vPosition -= float4(vOffset.x, vOffset.y, 0.f, 0.f);
    output.vWorldPos = output.vPosition.xyz;
    
    // View, Proj 행렬 
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vColor = _in.vColor;
    
    return output;
}

#endif