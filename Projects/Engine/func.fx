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
            float fTheta = (fDist / info.fRadius) * (PI / 2.f); // 0~1���� ���� 0~pi/2�� ��Ī��Ŵ
            fAttenue = saturate(cos(fTheta));
            
            _output.vColor += info.Color.vColor * fAttenue;
        }
    }
    
    // Spot Light
    else if (info.LightType == 2)
    {
        float fDist = distance(info.vWorldPos.xy, _vWorldPos.xy);
        
        if (fDist < info.fRadius)
        {
            // 1. ������ ���� �ȼ�, �� �� ������ ��(float) ����
            // - ��ũź��Ʈ�� ����ؼ� ���� ���� �ְ� (��Ȯ�� ��, ���� �δ�)
            // - ������ ������ ����ؼ� ���� ���� ���� (���� ���̰� üũ, ���� ����)
            float2 pixelVec = (_vWorldPos.xy - info.vWorldPos.xy);
            float fVecScale = (sqrt(pow(info.vWorldDir.x, 2) + pow(info.vWorldDir.y, 2))) * (sqrt(pow(pixelVec.x, 2) + pow(pixelVec.y, 2))); // �� vector�� ũ�� ����
            float fDot = (pixelVec.x * info.vWorldDir.x) + (pixelVec.y * info.vWorldDir.y); // ������ ���� (2D: z�� 0+0���� �ΰ� xy�� ���)
            float fAngle = acos(fDot / fVecScale); // �� ����
            
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
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (g_time /*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
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

#endif