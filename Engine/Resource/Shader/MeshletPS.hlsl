#include "Meshlet.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    //float32_t4 textureColor = gTexture.Sample(gSampler, input.texCoord);
    
    return input.color;
}