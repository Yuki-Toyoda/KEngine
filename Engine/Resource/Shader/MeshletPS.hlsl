#include "Meshlet.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // テクスチャ色を求める
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texCoord);
    
    // 入力された頂点色とテクスチャ色を掛けて最終的な色を求める
    float32_t4 finalColor = input.color * textureColor;
    
    return finalColor;
}