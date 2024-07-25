#include "../Constant.hlsli"

struct VertexShaderOutput
{
    float32_t4 position : SV_Position;
    float32_t3 texcoord : TEXCOORD0;
};