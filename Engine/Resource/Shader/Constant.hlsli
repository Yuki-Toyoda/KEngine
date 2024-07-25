struct ConstantData
{
    float32_t4x4 WorldViewProj;
    float32_t4x4 ViewProjection2D;
    float32_t3   WorldPosition;
    uint32_t     DrawMeshlets;
    int32_t      ToonMapIndex;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float32_t  intensity;
};

struct WorldTransform
{
    float32_t4x4 world;
};

struct Material
{
    float32_t4x4 uvTransform;
    float32_t4   color;
    int32_t      enableLighting;
    int32_t      textureIndex;
};

struct VertexData
{
    float32_t4 pos;
    float32_t2 texCoord;
    float32_t3 normal;
    float32_t4 color;
    float32_t4 weight;
    int32_t4   jointIndex;
    int32_t    materialIndex;
};

struct Meshlet
{
    uint32_t VertCount;
    uint32_t VertOffset;
    uint32_t PrimCount;
    uint32_t PrimOffset;
};

struct VertexOutPut
{
    float32_t4 pos      : SV_POSITION0;
    float32_t2 texCoord : TEXCOORD0;
    float32_t3 normal   : NORMAL0;
    float32_t3 world    : POSITION0;
    float32_t4 color    : COLOR0;
    int32_t    mIndex   : INDEX0;
};