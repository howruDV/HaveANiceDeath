#ifndef DOWNSAMPLING
#define DOWNSAMPLING

#define INTERPOL_LEVEL 3
Texture2D<float4> inputTexture : register(t0);      // ū ������ �ؽ���
RWTexture2D<float4> outputTexture : register(u0);   // ���� ������ �ؽ���

[numthreads(32, 32, 1)]
void CS_DownSampling(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint inputWidth, inputHeight;
    inputTexture.GetDimensions(inputWidth, inputHeight);

    uint outputWidth, outputHeight;
    outputTexture.GetDimensions(outputWidth, outputHeight);
    
    if (outputWidth <= dispatchThreadID.x || outputHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    // �Է� �ؽ�ó���� �ȼ� ���� �о�� ��ġ ���
    float2 inputUV = float2(dispatchThreadID.xy) / float2(outputWidth, outputHeight);
    float2 Location = inputUV * float2(inputWidth, inputHeight);
    Location = round(Location);
    
    // �Է� �ؽ�ó���� ���ø��Ͽ� ���ο� �ؽ�ó�� ����
    float4 color = inputTexture.Load(int3(Location.x, Location.y, 0));
    
    for (uint i = 1; i <= INTERPOL_LEVEL; ++i)
    {
        if (i / 2 == 0)
        {
            color += inputTexture.Load(int3(Location.x - i, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x - i, Location.y + i, 0));
            color += inputTexture.Load(int3(Location.x + i, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x + i, Location.y + i, 0));
        }
        else
        {
            color += inputTexture.Load(int3(Location.x + i, Location.y, 0));
            color += inputTexture.Load(int3(Location.x - i, Location.y, 0));
            color += inputTexture.Load(int3(Location.x, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x, Location.y + i, 0));
        }
    }
    
    color /= (INTERPOL_LEVEL * 4 + 1);
    outputTexture[dispatchThreadID.xy] = color;
}
#endif