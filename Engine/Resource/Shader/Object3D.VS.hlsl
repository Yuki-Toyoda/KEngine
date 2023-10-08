#include "Object3D.hlsli"
// 
VertexShaderOutput main(VertexShaderInput input)
{
    // 
    VertexShaderOutput output;
    // 
    output.position = mul(input.position, mat);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3)world));
    // 
    return output;
}