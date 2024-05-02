#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

void CalLight2D(float3 _vWorldPos, int _LightIdx, inout tLightColor _output)
{
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (info.LightType == 0)
        _output.vAmbient += info.Color.vAmbient;
    
    // Point Light
    else if (info.LightType == 1)
    {
        float fAttenue = 1.f;
        float fDist = distance(info.vWorldPos.xy, _vWorldPos.xy);
        
        if (fDist < info.fRadius)
        {
            //float fTheta = (fDist / info.fRadius) * (PI / 2.f); // 0~1사이 값을 0~pi/2로 매칭시킴
            //fAttenue = saturate(cos(fTheta));
            fAttenue = 1 - (fDist / info.fRadius);
            
            _output.vColor += info.Color.vColor * fAttenue;
        }
    }
    
    // Spot Light
    else if (info.LightType == 2)
    {
        float fDist = distance(info.vWorldPos.xy, _vWorldPos.xy);
        
        if (fDist < info.fRadius)
        {
            // 1. 광원과 현재 픽셀, 두 점 사이의 각(float) 구함
            // - 아크탄젠트를 사용해서 구할 수도 있고 (정확한 값, 연산 부담)
            // - 백터의 내적을 사용해서 구할 수도 있음 (벡터 사이각 체크, 연산 쉬움)
            float2 pixelVec = (_vWorldPos.xy - info.vWorldPos.xy);
            float fVecScale = (sqrt(pow(info.vWorldDir.x, 2) + pow(info.vWorldDir.y, 2))) * (sqrt(pow(pixelVec.x, 2) + pow(pixelVec.y, 2))); // 두 vector의 크기 구함
            float fDot = (pixelVec.x * info.vWorldDir.x) + (pixelVec.y * info.vWorldDir.y); // 내적값 구함 (2D: z는 0+0으로 두고 xy만 계산)
            float fAngle = acos(fDot / fVecScale); // θ 구함
            
            if (fAngle <= info.fAngle / 2.f)
            {
                float fAttenu = 1.f;
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
                _output.vColor += info.Color.vColor * fAttenu;
            }
        }
    }
}

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_time * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_time /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)].xyz * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

float4 Rotation(float4 Transform, float Angle)
{
    Angle *= PI / 180.f;
    float fSinAngle = sin(Angle);
    float fCosAngle = cos(Angle);
    
    row_major matrix M = matrix(
        float4(fCosAngle, -fSinAngle, 0.0f, 0.0f),
        float4(fSinAngle, fCosAngle, 0.0f, 0.0f),
        float4(0.0f, 0.0f, 1.0f, 0.0f),
        float4(0.0f, 0.0f, 0.0f, 1.0f));
    
    return mul(Transform, M);
}

#endif