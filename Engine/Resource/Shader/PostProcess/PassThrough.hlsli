struct VertexOutput
{
    float32_t4 position : SV_POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};

struct Parameter
{
    int rWidth;
    int rHeight;
};

ConstantBuffer<Parameter> gPara : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);
