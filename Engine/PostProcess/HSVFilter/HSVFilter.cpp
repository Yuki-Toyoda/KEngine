#include "HSVFilter.h"
#include "Engine/Base/DirectXCommon.h"

void HSVFilter::Init()
{
	// 各種変数のリセット
	use_ = true;
	hsv_.hue		= 0.0f;
	hsv_.saturation = 0.0f;
	hsv_.value		= 0.0f;

	// バッファの初期化を行う
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void HSVFilter::Update()
{
	// バッファ内のデータを更新する
	*buffer_.data_ = hsv_;
}

void HSVFilter::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を書き込む
	std::string str = R"(

struct HSV {
	float32_t hue;
	float32_t saturation;
	float32_t value;
};

ConstantBuffer<HSV> hsvData : register(b${v});

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
)";

	// 変数でバインドの値を変更する
	size_t pos;
	while ((pos = str.find("${v}")) != std::string::npos) {
		str.replace(pos, 4, std::to_string(bindIndex_));
	}

	// シェーダーファイルに書き込む
	*stream << str;

	// 定数用パラメータ生成する
	root->CreateCBVParameter(bindIndex_, D3D12_SHADER_VISIBILITY_PIXEL);

	// iを加算
	*i += 1;
}

void HSVFilter::WriteProcess(std::ofstream* stream)
{
	// 
	*stream << R"(
	HSV tempHSV = RGBToHSV(output.rgb);

	tempHSV.hue += hsvData.hue;
	tempHSV.saturation += hsvData.saturation;
	tempHSV.value += hsvData.value;
	
	tempHSV.hue = WrapValue(tempHSV.hue, 0.0f, 1.0f);
	tempHSV.saturation = saturate(tempHSV.saturation);
	tempHSV.value = saturate(tempHSV.value);
	
	output.rgb = HSVToRGB(tempHSV);
	output.a = 1.0f;
)";
}

void HSVFilter::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// コマンドリストに定数バッファのアドレスを書き込む
	cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void HSVFilter::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("HSVFilter")) {
		// 有効化フラグ
		ImGui::Checkbox("Use", &use_);
		// HSV
		ImGui::DragFloat3("HSV", &hsv_.hue, 0.01f, -1.0f, 1.0f);
		ImGui::TreePop();
	}
}
