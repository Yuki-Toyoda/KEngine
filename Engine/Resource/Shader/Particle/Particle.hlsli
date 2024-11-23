#include "../Compute/Random.hlsli"

struct Particle
{
    float32_t4  color;
    float32_t3  translate;
    float32_t   currentTime;
    float32_t3  scale;
    float32_t   lifeTime;
    float32_t3  rotate;
    float32_t3  velocity;
};

struct EmitterSphere
{
    float32_t3  translate;
    float32_t   radius;
    float32_t   frequency;
    float32_t   frequencyTime;
    int32_t     count;
    int32_t     emit;
};

struct PerFrame
{
    float32_t time;
    float32_t deltaTime;
};

struct InfoData
{
    int32_t instanceCount;
    int32_t isBillboard;
};

ConstantBuffer<EmitterSphere>   emitter       : register(b0);
ConstantBuffer<PerFrame>        perFrame      : register(b1);
ConstantBuffer<InfoData>        info          : register(b2);
RWStructuredBuffer<Particle>    gParticles    : register(u0);
RWStructuredBuffer<int32_t>     freeListIndex : register(u1);
RWStructuredBuffer<uint32_t>    freeList      : register(u2);