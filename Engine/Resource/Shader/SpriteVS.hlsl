#include "Sprite.hlsli"
// 
VertexShaderOutput main(VertexShaderInput input)
{
    // 
    VertexShaderOutput output;
    // 
    output.position = mul(input.position, mat);
    output.texcoord = input.texcoord;
    //
    return output;
}
