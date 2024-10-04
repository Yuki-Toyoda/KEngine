#include "GrayScale.h"
#include "Engine/Base/DirectXCommon.h"

void GrayScale::Init()
{
	// かけ具合リセット
	intensity_ = 0.0f;

	// バッファ初期化
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void GrayScale::Update()
{
	// バッファにデータを書き込み
	*buffer_.data_ = intensity_;
}

void GrayScale::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を記述
	std::string str = R"(
struct GrayScaleData {
	float32_t intensity;
};

ConstantBuffer<GrayScaleData> gsData : register(b${v});
    
float32_t3 GrayScale(float32_t3 color)
{
    // グレースケール色を設定する
    float32_t3 result = dot(color, float32_t3(0.2125f, 0.7154f, 0.0721f));
    return lerp(color, result, gsData.intensity);
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

void GrayScale::WriteProcess(std::ofstream* stream)
{
	// シェーダーファイルに書き込む
	*stream << R"(
	output.rgb = GrayScale(output.rgb);
)";
}

void GrayScale::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// コマンドリストに定数バッファのアドレスを書き込む
	cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void GrayScale::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("GrayScale")) {
		// 有効化フラグ
		ImGui::Checkbox("Enable", &use_);

		// 強度をImGuiでいじる
		ImGui::DragFloat("Intensity", &intensity_, 0.01f, -1.0f, 1.0f);

		// ツリーノード終了
		ImGui::TreePop();
	}
}
