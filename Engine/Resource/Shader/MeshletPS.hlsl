#include "Meshlet.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    return input.color;
}