#include "Trail.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // テクスチャ色を求める
    float32_t4 output = gTexture[material.textureIndex].Sample(gSampler, input.texCoord);
    // そのまま出力
    return output * material.color;
}