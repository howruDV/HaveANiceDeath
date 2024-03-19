#ifndef _DEBUG
#define _DEBUG

#include "value.fx"

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

VS_OUT VS_DebugShape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

static float g_Thickness = 0.1f;
float4 PS_DebugShape(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // outline
    // - ���1: PS����UV�� ����
    //if (g_Thickness <= _in.vUV.x && _in.vUV.x <= (1.f - g_Thickness)
    //    && g_Thickness <= _in.vUV.y && _in.vUV.y <= (1.f - g_Thickness))
    //{
    //    discard;
    //}
    // - ���2: rasterizer�� wireframe render�� ����
    // - ���3: topology�� line strip���� ����
    
    vOutColor = g_vec4_0;
    vOutColor.a = 1.f;
    return vOutColor;
}

#endif