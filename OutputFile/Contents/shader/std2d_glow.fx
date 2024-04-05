#ifndef _PLAYER
#define _PLAYER

#include "value.fx"
#include "func.fx"

#define AnimDir g_int_0
#define GlowEnable g_int_1
#define GlowColor g_vec4_0
#define Threshold g_float_0

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

VS_OUT VS_Std2D_Glow(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // World�� �̵�
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // Offset ��������
    float2 vOffset = { g_vOffset.x * g_vAtlasSize.x, g_vOffset.y * g_vAtlasSize.y };
    
    
    // Animation ������
    if (AnimDir == -1)
    {
        output.vUV = float2((1.f - _in.vUV.x), _in.vUV.y);
        output.vPosition -= float4(-vOffset.x, vOffset.y, 0.f, 0.f);
    }
    else
    {
        output.vUV = _in.vUV;
        output.vPosition -= float4(vOffset.x, vOffset.y, 0.f, 0.f);
    }
    
    
    // World ����
    output.vWorldPos = output.vPosition;
    
    // View, Proj ��� 
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vColor = _in.vColor;
    
    return output;
}

struct PS_Std2D_Glow_Output
{
    float4 RenderTarget : SV_Target0;
    float4 GlowTarget : SV_Target1;
};


PS_Std2D_Glow_Output PS_Std2D_Glow(VS_OUT _in) : SV_Target
{
    PS_Std2D_Glow_Output output;
    
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        float2 vLeftTop = g_vLeftTop;
        float2 vUV = g_vLeftTop + (_in.vUV * g_vCutSize);
        
        // ���ø�
        vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
    }
    else
    {
        // Defalut Texture
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        
        // ����Ÿ ���� ������ discard
        //saturate 0 ~ 1 �� ���� �ʰ� ����
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // �ȼ� ���̴��� �߰��� ���ó��
            discard; //clip(-1);            
        }
        
    }
    
    // �ȼ� ���� Relative luminance�� ���
    float RelativeLuminance = dot(vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    
    // �ش� ���� Threshold���� ũ�ٸ� Glow Color ���
    if (true)
    {
        output.GlowTarget = GlowColor;
    }
    else
    {
        output.GlowTarget = float4(0.f, 0.f, 0.f, 1.f);
    }

        
    // ���� ó��
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);

    if (0.f == vColor.a)
        discard;
    
    
    // ���� ����Ÿ�ٿ� ���
    output.RenderTarget = vColor;
   
    return output;
}
#endif