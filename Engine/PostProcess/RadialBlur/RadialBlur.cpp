#include "RadialBlur.h"
#include "Engine/Base/DirectXCommon.h"

void RadialBlur::Init()
{
	// データリセット
	data_.center		= { 0.5f, 0.5f };	// 中心点
	data_.blurStrength	= 0.0f;				// ブラー強さ

	// バッファの初期化
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void RadialBlur::Update()
{
	// バッファにデータを書き込む
	*buffer_.data_ = data_;
}

void RadialBlur::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を記述
	std::string str = R"(
struct RadialData {
	float32_t2 center;
	float32_t blurStrength;
};

ConstantBuffer<RadialData> raData : register(b${v});
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

void RadialBlur::WriteProcess(std::ofstream* stream)
{
    *stream << R"(
	if(raData.blurStrength > 0.0f){
		const int32_t kNumSamples = 10;
		float32_t2 direction = input.texcoord - raData.center;
		for(int32_t sampleIndex = 0; sampleIndex < kNumSamples; sampleIndex++){
			float32_t2 texcoord = input.texcoord + direction * raData.blurStrength * float32_t(sampleIndex);
			output.rgb += gTexture.Sample(gSampler, texcoord).rgb;
		}

		output.rgb *= rcp(kNumSamples);
	}
)";
}

void RadialBlur::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// コマンドリストに定数バッファのアドレスを書き込む
	cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void RadialBlur::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("RadialBlur")) {
		// 有効化フラグ
		ImGui::Checkbox("Enable", &use_);

		// ブラーの中心座標をいじる
		ImGui::DragFloat2("Center", &data_.center.x, 0.01f, 0.0f, 1.0f);
		// ブラーの強さをいじる
		ImGui::DragFloat("Intensity", &data_.blurStrength, 0.001f, 0.0f, 0.1f);

		// ツリーノード終了
		ImGui::TreePop();
	}
}
