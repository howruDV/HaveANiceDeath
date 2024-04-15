#ifndef _STD2D_MASK
#define _STD2D_MASK

#include "value.fx"
#include "func.fx"

#define MixColor g_vec4_0
#define MoveUV g_vec2_0
#define MaskImg g_tex_1

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

PS_OUT PS_Std2D_Mask(VS_OUT _in) : SV_Target
{
    // 1. sampling
    PS_OUT output;
    float4 vColor;
    
    // masking
    if (g_btex_1)
    {
        float4 vMasking = g_tex_1.Sample(g_sam_2, _in.vUV);
        if (vMasking.a <= 0.1f)
           discard;
    }
    _in.vUV += MoveUV;
    
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
        float2 vUV = vBackgroundLeftTop + _in.vUV * g_vBackgroundSize; //UV�� �� �������� ũ�⸦ ������� �ϹǷ�, �������� ũ�⸸ŭ ������� ��
        
        // ���������� �̹����� ����� �׸��� ����
        // ��, ������ �κп� ���ؼ��� �� �׸�
        if (vUV.x < g_vLeftTop.x || vUV.x > g_vLeftTop.x + g_vCutSize.x
            || vUV.y < g_vLeftTop.y || vUV.y > g_vLeftTop.y + g_vCutSize.y)
            discard;
        else
            vColor = g_anim2d_tex.Sample(g_sam_2, vUV);
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_2, _in.vUV);
        }
    }
    
    // 2. lighting
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
        CalLight2D(_in.vWorldPos, i, LightColor);
    
    vColor.rgb *= LightColor.vColor.rgb + LightColor.vAmbient.rgb;
    
    if (MixColor.a != 0.f)
        vColor.rgb *= MixColor.rgb;
    
    // 3. cut masked
    if (vColor.a == 0.f)
        discard;
    
    // render target 0
    output.RenderTarget = vColor;
    return output;
}

#endif