#ifndef DOWNSAMPLING
#define DOWNSAMPLING

// ū ������ �ؽ���
Texture2D<float4> inputTexture : register(t0);
// ���� ������ �ؽ���
RWTexture2D<float4> outputTexture : register(u0);


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
    uint2 inputPixelPos = uint2(dispatchThreadID.xy * 2);
    
    float2 inputUV = float2(dispatchThreadID.xy) / float2(outputWidth, outputHeight);
    float2 Location = inputUV * float2(inputWidth, inputHeight);
    
    // �Է� �ؽ�ó���� ���ø��Ͽ� ���ο� �ؽ�ó�� ����
    float4 color = inputTexture.Load(int3(Location.x, Location.y, 0));
    
    outputTexture[dispatchThreadID.xy] = color;
}

#endif