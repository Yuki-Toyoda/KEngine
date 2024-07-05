#include "PostProcessor.h"
#include "../Base/DirectXCommon.h"

void PostProcessor::Init()
{
	// ポストプロセス初期化
	grayScale_.Init();

	// シェーダー生成開始
	CreateShaderFile();
}

void PostProcessor::Update()
{
	// ポストプロセスの更新を行う
	grayScale_.Update(); // グレースケール更新
}

void PostProcessor::CreateShaderFile()
{
	// ルートシグネチャの生成用にデバイスとDXCの取得
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	DXC* dxc = DirectXCommon::GetInstance()->GetRendererManager()->GetDXC();

	// 全ポストプロセスの取得
	std::vector<IPostProcess*> processes = GetAllProcess();
	// 取得し終わった段階で配列をクリア
	processes_.clear();

	// シェーダーファイル生成
	std::ofstream shader;
	shader.open("Engine/Resource/Shader/PostProcess/PostProcess.PS.hlsl", std::ios::trunc);

	// 定義記述
	shader << R"(
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

)";

	// ルートシグネチャのパラメータ生成
	root_.Init();
	root_.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);				// 汎用データ
	root_.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_PIXEL); // テクスチャデータ
	root_.CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_PIXEL); // 深度マップ
	root_.CreateSampler(0);													// サンプラー

	/// 各ポストプロセスのパラメータも追加する
	// バインド番号指定用
	int b = 1;
	// ポストプロセス分ループさせる
	for (int i = 0; i < processes.size(); i++) {
		// ポストプロセスを使用するなら
		if (processes[i]->use_) {
			// バインド設定をシグネチャにセットする
			processes[i]->WriteBinding(&shader, &root_, &b);
		}
	}
	// RootSignatureにデバイスを渡す
	root_.SetDevice(device->GetDevice());
	// パラメータ追加し次第RootSignatureを生成する
	root_.CreateRootSignature();

	// メイン処理の書き込み
	shader << R"(

float32_t4 main(PSInput input) : SV_TARGET {
	float32_t4 output;
	float32_t2 uv = input.texcoord;

	output = gTexture.Sample(gSampler, uv);

)";

	// メイン関数内の処理を書き込む
	// ポストプロセス分ループさせる
	for (int i = 0; i < processes.size(); i++) {
		// ポストプロセスを使用するなら
		if (processes[i]->use_) {
			// バインド設定をシグネチャにセットする
			processes[i]->WriteProcess(&shader);
			// プロセス配列を追加
			processes_.push_back(processes[i]);
		}
	}

	// 出力処理の書き込み
	shader << R"(
	return output;
}
)";

	// シェーダー作成完了
	shader.close();

	// PSOの生成を行う
	pso_.VertInit(root_.GetRootSignature(), dxc)
		.SetDepthStencilState(false)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetVertPixelShader("Engine/Resource/Shader/PostProcess/PostProcess.PS.hlsl")
		.VertBuild(device->GetDevice());
}

void PostProcessor::PreCommands(ID3D12GraphicsCommandList6* cmdList, RenderResource* target)
{
	// 全プロセス分ループ
	for (int i = 0; i < processes_.size(); i++) {
		// 事前処理を行う
		processes_[i]->PreCommand(cmdList, target);
	}
}

void PostProcessor::DisplayImGui()
{
	// ポストプロセス関連のImGuiを表示
	if (ImGui::TreeNode("PostProcess")) {
		// ポストプロセス関連のImGuiを表示
		grayScale_.DisplayImGui(); // グレースケール

		// シェーダーの再読み込み
		if (ImGui::Button("Reload Shader")) {
			CreateShaderFile();
		}

		// ツリーノード終了
		ImGui::TreePop();
	}
}

void PostProcessor::SetToCommandList(ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにRoot SignatureとPSOをセットする
	list->SetGraphicsRootSignature(root_.GetRootSignature()); // Root Signature
	list->SetPipelineState(pso_.GetState());				  // PSO

	// 共通化を行い、まとめて処理を行う
	int offset = 2;
	// 全ポストプロセス分ループ
	for (int i = 0; i < processes_.size(); i++) {
		// コマンドリストに各ポストプロセスの値をセットする
		processes_[i]->SetToCommandList(list, &offset);
	}
}

std::vector<IPostProcess*> PostProcessor::GetAllProcess()
{
	// 返還用配列
	std::vector<IPostProcess*> result;
	// 返還用配列に各ポストプロセスの参照を渡す
	result.push_back(&grayScale_); // グレースケール
	// 配列を返還する
	return result;
}
