#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

#define GlowEnable g_int_0
#define GlowColor g_vec4_0
#define Threshold g_float_0

StructuredBuffer<FParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<FParticleModule> g_ParticleModule : register(t21);
StructuredBuffer<float4> g_AnimOffset : register(t22);

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

struct PS_OUT
{
    float4 RenderTarget : SV_Target0;
    float4 GlowTarget : SV_Target1;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.InstID = _in.InstID;
    
    return output;
}

// Gemotery Shader: 중간에 정점 생성시킬 수 있는 쉐이더
// 1. 비활성화 파티클 연산 중단
// 2. 빌보드 쉽게 구현
// - point 하나를 중심점으로 받아, rect mesh 만들 수 있게 vertex 찍음
// - rect 찍기 위해서는 정점 6개가 필요함 (Index Buffer의 역할하는것 없으므로 겹쳐도 어쩔 수 X)
[maxvertexcount(12)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    FParticle particle = g_ParticleBuffer[(int) _in[0].InstID];
    if (!particle.Active)
        return;
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    GS_OUT output_cross[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    float3 vWorldPos = particle.vWorldPos.xyz;                  // world pos 
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);   // view pos
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    output[0].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
   
    output_cross[0].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[1].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[2].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    output_cross[3].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    
    output_cross[0].vUV = output[0].vUV = float2(0.f, 0.f);
    output_cross[1].vUV = output[1].vUV = float2(1.f, 0.f);
    output_cross[2].vUV = output[2].vUV = float2(1.f, 1.f);
    output_cross[3].vUV = output[3].vUV = float2(0.f, 1.f);
    
    // Flip & Offset
    if (g_UseAnim2D)
    {
        float2 vOffset = { g_AnimOffset[particle.AnimFrmIdx].x * g_vAtlasSize.x, g_AnimOffset[particle.AnimFrmIdx].y * g_vAtlasSize.y };
    
        for (int i = 0; i < 4; ++i)
        {
            if (g_FlipAnimXY & (1 << 1))
            {
                output_cross[i].vUV.x = output[i].vUV.x = (1.f - output[i].vUV.x);
                vOffset.x *= -1;
            }
        
            if (g_FlipAnimXY & (1 << 0))
            {
                output_cross[i].vUV.y = output[i].vUV.y = (1.f - output[i].vUV.y);
                vOffset.y *= -1;
            }
            
            output[i].vPosition.xy -= vOffset;
            output_cross[i].vPosition.xy -= vOffset;
        }
    }
    
    if (particle.vWorldRotation.z != 0.f)
    {
        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition = Rotation(output[i].vPosition, particle.vWorldRotation.z);
            output_cross[i].vPosition = Rotation(output_cross[i].vPosition, particle.vWorldRotation.z);
        }
    }
    
    // -------------------------
    // Module : Render Module (Before Projection)
    // -------------------------
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬
        if (g_ParticleModule[0].VelocityAlignment)
        {
            float3 vR = normalize(mul(float4(particle.vVelocity.xyz, 0.f), g_matView).xyz);
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));
            float3x3 vRot = { vR, vU, vF };
            
            for (int i = 0; i < 4; ++i) // 4개 점 = rect 회전
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
                output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
            }
        }
    }
    
    // local space -> view space -> screen space
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;                    // local space -> (world space) -> view space
        output[i].vPosition = mul(output[i].vPosition, g_matProj);  // view space -> screen space
        
        output_cross[i].vPosition.xyz += vViewPos.xyz;
        output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        
        output[i].InstID = _in[0].InstID;
        output_cross[i].InstID = _in[0].InstID;
    }
    
    // return vertex
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    // -------------------------
    // Module : Render Module (After Projection)
    // -------------------------
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬
        if (g_ParticleModule[0].VelocityAlignment)
        {
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[1]);
            _OutStream.Append(output_cross[2]);
            _OutStream.RestartStrip();
            
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[2]);
            _OutStream.Append(output_cross[3]);
            _OutStream.RestartStrip();
        }
    }
}

PS_OUT PS_Particle(GS_OUT _in) : SV_Target
{
    FParticle particle = g_ParticleBuffer[_in.InstID];
    FParticleModule module = g_ParticleModule[0];
    PS_OUT output;
    float4 vOutColor = g_ParticleBuffer[(uint) _in.InstID].vColor;
    vOutColor.a = 1.f;

    // 1. sampling
    if (g_UseAnim2D)
    {
        if (!(particle.AnimFinish && !module.AnimRepeat))
        {
            int2 AtlasFrmSize = g_vAtlasSize / (g_vCutSize * g_vAtlasSize);
            int FrmXIdx = particle.AnimFrmIdx % AtlasFrmSize.x;
            int FrmYIdx = particle.AnimFrmIdx / AtlasFrmSize.x;
            float2 vLeftTop = float2(g_vCutSize.x * FrmXIdx, g_vCutSize.y * FrmYIdx);
        
            float2 vBackgroundLeftTop = vLeftTop + (g_vCutSize / 2.f) - (g_vBackgroundSize / 2.f);
            float2 vUV = vBackgroundLeftTop + _in.vUV * g_vBackgroundSize;
        
            if (vUV.x < vLeftTop.x || vUV.x > vLeftTop.x + g_vCutSize.x
            || vUV.y < vLeftTop.y || vUV.y > vLeftTop.y + g_vCutSize.y)
                discard;
            else
            {
                float4 vSampleColor = g_anim2d_tex.Sample(g_sam_0, vUV);
                vOutColor.rgb *= vSampleColor.rgb;
                vOutColor.a = vSampleColor.a;
            }
        }
    }
    else
    {
        if (g_btex_0)
        {
            float4 vSampleColor = g_tex_0.Sample(g_sam_0, _in.vUV);
            vOutColor.rbg *= vSampleColor.rbg;
            vOutColor.a = vSampleColor.a;
        }
    }
    
    // Relative luminance
    float RelativeLuminance = dot(vOutColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    
    if (GlowEnable && (Threshold < RelativeLuminance))
    {
        output.GlowTarget = GlowColor;
    }
    else
    {
        output.GlowTarget = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    // -------------------------
    // Module : Render Module (PS)
    // -------------------------
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // Alpha 변화 (0: Off, 1: NormalizedAge, 2: Age)
        if (module.AlphaBasedLife == 1)
        {
            vOutColor.a *= lerp(1.f, 0.f, particle.NormalizedAge);
        }
        else if (module.AlphaBasedLife == 2)
        {
            float fRatio = particle.Age / module.AlphaMaxAge;
            vOutColor.a *= saturate(1.f - clamp(fRatio, 0.f, 1.f));
        }
    }
    
    // render target
    output.RenderTarget = vOutColor;
    return output;
}

#endif