#include "PassThrough.hlsli"
#include "GrayScale.hlsli"

ConstantBuffer<IntensityData> gIntensity : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const float32_t kKernel3x3[3][3] =
{
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f }
};

float32_t4 main(VertexOutput input) : SV_TARGET
{
    // テクスチャ解像度取得
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
	// 最終結果
    float32_t4 output;
	
    // 出力色リセット
    output.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.a = 1.0f;
    
    for (int32_t x = 0; x < 3; x++)
    {
        for (int32_t y = 0; y < 3; y++)
        {
            // 現在のTexCoord算出
            float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            // 色に 1 / 9 かけて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.rgb += fetchColor * kKernel3x3[x][y];
        }
    }
    
    // 出力する
    return output;
}