struct Vertex
{
    float4 pos : SV_Position;
    float3 color : COLOR0;
};

float4 main(Vertex input) : SV_TARGET
{
    return float4(input.color, 1.0);
}