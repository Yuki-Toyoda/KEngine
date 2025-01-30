#include "SkyBox.hlsli"

TextureCube<float32_t4> gTexture : register(t2);
SamplerState gSampler : register(s0);

float32_t4 main(VertexShaderOutput input) : SV_TARGET
{
    // テクスチャ色を求める
    float32_t4 output = gTexture.Sample(gSampler, input.texcoord);
    // そのまま出力
    return output;
}