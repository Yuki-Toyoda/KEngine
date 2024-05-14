#include "PassThrough.hlsli"
#include "GrayScale.hlsli"

ConstantBuffer<IntensityData> gIntensity : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);

float32_t4 main(VertexOutput input) : SV_TARGET
{
	// 最終結果
    float32_t4 output;
	
    // レンダリングする座標
    float2 uv = input.texcoord;

    // サンプリング
    output = gTexture.Sample(gSampler, uv);
    float2 center = float2(0.5f, 0.5f); // 中心座標
    float2 delta = center - input.texcoord; // ピクセルと中心の距離
    float vignette = saturate(1.0f - length(delta) * gIntensity.intensity); // ビネットの効果を計算
    output = float4(output.rgb * vignette, output.a); // カラーにビネットを適用
    
    // 出力する
    return output;
}