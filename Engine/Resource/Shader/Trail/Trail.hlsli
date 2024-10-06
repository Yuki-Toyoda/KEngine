struct ConstantData
{
    float32_t4x4    WorldViewProj;
    float32_t4x4    ViewProjection2D;
    float32_t4x4    Billboard;
    float32_t3      WorldPosition;
    uint32_t        DrawMeshlets;
    int32_t         ToonMapIndex;
};

struct WorldTransform
{
    float32_t4x4 world;
};

struct Material
{
    float32_t4x4    uvTransform;
    float32_t4      color;
    int32_t         enableLighting;
    int32_t         textureIndex;
    float32_t       environmentCoefficient_;
    float32_t       dissolveStrength;
    float32_t3      dissolveEdgeColor;
    float32_t       dissolveEdgeThreshold;
};

struct VertexData
{
    float32_t4 pos;
    float32_t2 texCoord;
};

struct VertexOutPut
{
    float32_t4 pos      : SV_POSITION0;
    float32_t2 texCoord : TEXCOORD0;
};

ConstantBuffer<ConstantData>    cData       : register(b0);
ConstantBuffer<WorldTransform>  transform   : register(b1);
ConstantBuffer<Material>        material    : register(b2);

StructuredBuffer<VertexData> Vertices   : register(t0);
StructuredBuffer<uint32_t>   Indices    : register(t1);
Texture2D<float32_t4> gTexture[512]     : register(t2);

SamplerState gSampler : register(s0);