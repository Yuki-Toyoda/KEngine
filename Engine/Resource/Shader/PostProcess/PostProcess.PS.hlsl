
struct PSInput
{
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
};

struct Parameter
{
	int time;
	int rWidth;
	int rHeight;
};

ConstantBuffer<Parameter> gPara : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);


struct VignetteData {
	float32_t intensity;
};
ConstantBuffer<VignetteData> vData : register(b1);

float32_t Vignette(float32_t2 texcoord) {
	float32_t2 center = float2(0.5f, 0.5f);
    float32_t2 delta = center - texcoord;
    float vignette = saturate(1.0f - length(delta) * vData.intensity);
	return vignette;
}
struct GrayScaleData {
	float32_t intensity;
};

ConstantBuffer<GrayScaleData> gsData : register(b2);
    
float32_t3 GrayScale(float32_t3 color)
{
    // グレースケール色を設定する
    float32_t3 result = dot(color, float32_t3(0.2125f, 0.7154f, 0.0721f));
    return lerp(color, result, gsData.intensity);
}


float32_t4 main(PSInput input) : SV_TARGET {
	float32_t4 output;
	float32_t2 uv = input.texcoord;

	output = gTexture.Sample(gSampler, uv);


	float v = Vignette(input.texcoord);
	output = float32_t4(output.rgb * v, output.a);

	output.rgb = GrayScale(output.rgb);

	return output;
}
