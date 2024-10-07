#include "Trail.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // テクスチャ色を求める
    float32_t4 output = gTexture[material.textureIndex].Sample(gSampler, input.texCoord);
   
    // テクスチャ色の透明度が0.5以下、マテリアル透明度が0であればそのピクセルを描画しない
    if (output.a <= 0.5f || material.color.a <= 0.0f)
    {
        discard;
    }
    
    // そのまま出力
    return output * material.color;
}