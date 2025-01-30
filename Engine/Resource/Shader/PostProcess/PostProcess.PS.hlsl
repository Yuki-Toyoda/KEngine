
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

struct RadialData {
	float32_t2 center;
	float32_t blurStrength;
};

ConstantBuffer<RadialData> raData : register(b2);

struct VignetteData {
	float32_t intensity;
};
ConstantBuffer<VignetteData> vData : register(b3);

float32_t Vignette(float32_t2 texcoord) {
	float32_t2 center = float2(0.5f, 0.5f);
    float32_t2 delta = center - texcoord;
    float vignette = saturate(1.0f - length(delta) * vData.intensity);
	return vignette;
}
struct GrayScaleData {
	float32_t intensity;
};

ConstantBuffer<GrayScaleData> gsData : register(b4);
    
float32_t3 GrayScale(float32_t3 color)
{
    // グレースケール色を設定する
    float32_t3 result = dot(color, float32_t3(0.2125f, 0.7154f, 0.0721f));
    return lerp(color, result, gsData.intensity);
}


struct HSV {
	float32_t hue;
	float32_t saturation;
	float32_t value;
};

ConstantBuffer<HSV> hsvData : register(b5);

HSV RGBToHSV(float32_t3 rgb) {
	HSV result = { 0.0f, 0.0f, 0.0f };
	
	float32_t min = rgb.r;
	float32_t max = rgb.r;

	if(min > rgb.g){
		min = rgb.g;
	}
	if(min > rgb.b){
		min = rgb.b;
	}

	if(max < rgb.g){
		max = rgb.g;
	}
	if(max < rgb.b){
		max = rgb.b;
	}
	
	result.hue = max - min;

	if(result.hue > 0.0f){
		if(max == rgb.r){
			result.hue = (rgb.g - rgb.b) / result.hue;
			
			if(result.hue < 0.0f){
				result.hue += 6.0f;
			}
		}
		else if(max == rgb.g){
			result.hue = 2.0f + (rgb.b - rgb.r) / result.hue;
		}
		else{
			result.hue = 4.0f + (rgb.r - rgb.g) / result.hue;
		}
	}	

	result.hue /= 6.0f;

	result.saturation = (max - min);
	if(max != 0.0f){
		result.saturation /= max;
	}
	
	result.value = max;

	return result;
}

float32_t3 HSVToRGB(HSV hsv) {
	float32_t3 result = { 0.0f, 0.0f, 0.0f };
	result.r = hsv.value;
	result.g = hsv.value;
	result.b = hsv.value;

	if(hsv.saturation > 0.0f){
		float32_t h = hsv.hue * 6.0f;

		int32_t i = int32_t(h);
		float32_t f = h - float32_t(i);

		switch(i){
		case 0:
			result.g *= 1.0f - hsv.saturation * (1.0f - f);
			result.b *= 1.0f - hsv.saturation;
			break;
		case 1:
			result.r *= 1.0f - hsv.saturation * f;
			result.b *= 1.0f - hsv.saturation;
			break;
		case 2:
			result.r *= 1.0f - hsv.saturation;
			result.b *= 1.0f - hsv.saturation * (1.0f - f);
			break;
		case 3:
			result.r *= 1.0f - hsv.saturation;
			result.g *= 1.0f - hsv.saturation * f;
			break;
		case 4:
			result.r *= 1.0f - hsv.saturation * (1.0f - f);
			result.g *= 1.0f - hsv.saturation;
			break;
		case 5:
			result.g *= 1.0f - hsv.saturation;
			result.b *= 1.0f - hsv.saturation * f;
			break;
		}
	}

	return result;
}

float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange){
	float32_t range = maxRange - minRange;
	float32_t modValue = fmod(value - minRange, range);

	if(modValue < 0){
		modValue += range;
	}

	return minRange + modValue;
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

	if(raData.blurStrength > 0.0f){
		const int32_t kNumSamples = 10;
		float32_t2 direction = input.texcoord - raData.center;
		for(int32_t sampleIndex = 0; sampleIndex < kNumSamples; sampleIndex++){
			float32_t2 texcoord = input.texcoord + direction * raData.blurStrength * float32_t(sampleIndex);
			output.rgb += gTexture.Sample(gSampler, texcoord).rgb;
		}

		output.rgb *= rcp(kNumSamples);
	}

	float v = Vignette(input.texcoord);
	output = float32_t4(output.rgb * v, output.a);

	output.rgb = GrayScale(output.rgb);

	HSV tempHSV = RGBToHSV(output.rgb);

	tempHSV.hue += hsvData.hue;
	tempHSV.saturation += hsvData.saturation;
	tempHSV.value += hsvData.value;
	
	tempHSV.hue = WrapValue(tempHSV.hue, 0.0f, 1.0f);
	tempHSV.saturation = saturate(tempHSV.saturation);
	tempHSV.value = saturate(tempHSV.value);
	
	output.rgb = HSVToRGB(tempHSV);
	output.a = 1.0f;

	return output;
}
