
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
    float4 color;
};


struct Camera
{
    float4x4 mat;
    float3 worldPosition;
};


struct Material
{
    float4x4 uvTransform;
    int enableLighting;
    int enablePhongReflection;
    int enableBlinnPhonReflection;
    float shininess;
};


struct DirectionalLight
{
    float4x4 viewProjection;
    float4 color;
    float3 direction;
    float intensity;
};


struct IndexInfo
{
    uint vertex;
    uint cameraVP;
    uint worldMatrix;
    uint material;
    uint tex2d;
};


StructuredBuffer<IndexInfo> gIndex : register(t0);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b0);


StructuredBuffer<Vertex> gVertex : register(t1);

StructuredBuffer<Camera> gCameraVP : register(t2);

StructuredBuffer<float4x4> gWorldMatrix : register(t3);


StructuredBuffer<Material> gMaterial : register(t1);


Texture2D<float4> gTexture[] : register(t2);
SamplerState gSampler : register(s0);


struct VertexShaderOutput
{
    float4 position : SV_Position0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float3 worldPos : POSITION0;
    float3 cameraWorldPos : POSITION1;
    uint id : SV_InstanceID;
};