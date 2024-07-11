#include "PassThrough.hlsli"
#include "GrayScale.hlsli"

ConstantBuffer<IntensityData> gIntensity : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);

// グレースケール関数
float32_t3 GrayScale(float32_t3 color)
{
    // グレースケール色を設定する
    return dot(color, float32_t3(0.2125f, 0.7154f, 0.0721f));
}

float32_t4 main(VertexOutput input) : SV_TARGET
{
	// 最終結果
    float32_t4 output;
	
    // レンダリングする座標
    float2 uv = input.texcoord;

	// サンプリング
    output = gTexture.Sample(gSampler, uv);
    // グレースケール化
    float3 grayScale = GrayScale(output.rgb);
    // 原色からグレースケールまでの補間を線形補間で求める
    float3 colored = lerp(output.rgb, grayScale, gIntensity.intensity);
    // 出力色設定
    output = float4(colored, 1.0f);
    
    // 出力する
    return output;
}