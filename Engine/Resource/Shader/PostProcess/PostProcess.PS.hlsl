
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
SamplerState gPointSampler : register(s1);


struct GaussianData {
	float32_t intensity;
};

ConstantBuffer<GaussianData> gaData : register(b1);
    
float Gaussian(float x, float sigma)
{
    return exp(-0.5 * (x * x) / (sigma * sigma)) / (sqrt(2.0 * 3.1415926535897932384626433832795) * sigma);
}

struct VignetteData {
	float32_t intensity;
};
ConstantBuffer<VignetteData> vData : register(b2);

float32_t Vignette(float32_t2 texcoord) {
	float32_t2 center = float2(0.5f, 0.5f);
    float32_t2 delta = center - texcoord;
    float vignette = saturate(1.0f - length(delta) * vData.intensity);
	return vignette;
}
struct GrayScaleData {
	float32_t intensity;
};

ConstantBuffer<GrayScaleData> gsData : register(b3);
    
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
	output.a = 1.0f;


    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));

    if (gaData.intensity >= 1.0f)
    {
         int kernelRadius = int(ceil(2.0 * gaData.intensity));
        int kernelSize = kernelRadius * 2 + 1;

        float kernel[49];
        float totalWeight = 0.0;
        for (int i = -kernelRadius; i <= kernelRadius; ++i)
        {
            kernel[i + kernelRadius] = Gaussian(float(i), gaData.intensity);
            totalWeight += kernel[i + kernelRadius];
        }

        float4 color = float4(0, 0, 0, 0);

        for (int y = -kernelRadius; y <= kernelRadius; ++y)
        {
            for (int x = -kernelRadius; x <= kernelRadius; ++x)
            {
                float2 offset = float2(float(x), float(y)) * uvStepSize;
                float weight = kernel[x + kernelRadius] * kernel[y + kernelRadius];
                color += gTexture.Sample(gSampler, input.texcoord + offset) * weight;
        }

        color /= totalWeight;
    
        output = color;
        output.a = 1.0f;
    }
    }

	float v = Vignette(input.texcoord);
	output = float32_t4(output.rgb * v, output.a);

	output.rgb = GrayScale(output.rgb);

	return output;
}
