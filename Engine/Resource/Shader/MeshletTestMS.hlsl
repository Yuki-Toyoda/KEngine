cbuffer ViewProjectionCBuffer : register(b0)
{
    matrix viewProjection;
};

struct Vertex
{
    float4 pos : SV_Position;
    float3 color : COLOR0;
};

float4 TransformPosition(float4 v)
{
    return mul(v, viewProjection);
}

static float4 cubeVertices[] =
{
    float4(-0.5f, -0.5f, -0.5f, 1.0f),
    float4(-0.5f, -0.5f, 0.5f, 1.0f),
    float4(-0.5f, 0.5f, -0.5f, 1.0f),
    float4(-0.5f, 0.5f, 0.5f, 1.0f),
    float4(0.5f, -0.5f, -0.5f, 1.0f),
    float4(0.5f, -0.5f, 0.5f, 1.0f),
    float4(0.5f, 0.5f, -0.5f, 1.0f),
    float4(0.5f, 0.5f, 0.5f, 1.0f),
};

static float3 cubeColors[] =
{
    float3(0.0f, 0.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f),
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 1.0f, 1.0f),
    float3(0.0f, 0.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f),
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 1.0f, 1.0f),
};

static uint3 cubeIndices[] =
{
    uint3(0, 2, 1),
    uint3(1, 2, 3),
    uint3(4, 5, 6),
    uint3(5, 7, 6),
    uint3(0, 1, 5),
    uint3(0, 5, 4),
    uint3(2, 6, 7),
    uint3(2, 7, 3),
    uint3(0, 4, 6),
    uint3(0, 6, 2),
    uint3(1, 3, 7),
    uint3(1, 7, 5),
};

[NumThreads(12, 1, 1)]
[OutputTopology("triangle")]
void main(
    in uint gtid : SV_GroupThreadID,
    out vertices Vertex verts[8],
    out indices uint3   tris[12]
)
{
    const uint numVertices = 8;
    const uint numPrimitives = 12;
    
    SetMeshOutputCounts(numVertices, numPrimitives);
    
    if (gtid < numVertices)
    {
        float4 pos = cubeVertices[gtid];
        verts[gtid].pos = TransformPosition(pos);
        verts[gtid].color = cubeColors[gtid];
    }
    
    tris[gtid] = cubeIndices[gtid];
}