struct VertexShaderOutput
{
    float32_t4 position : SV_Position;
    float32_t3 texcoord : TEXCOORD0;
};

struct ConstantData
{
    float32_t4x4 WorldViewProj;
    float32_t4x4 ViewProjection2D;
    uint32_t DrawMeshlets;
    int32_t ToonMapIndex;
};

struct WorldTransform
{
    float32_t4x4 world;
};