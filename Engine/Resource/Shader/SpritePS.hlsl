#include "Sprite.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// 
PixelShaderOutput main(VertexShaderOutput input)
{
    
    // 
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 
    output.color = color * textureColor;
    // 
    return output;
    
}