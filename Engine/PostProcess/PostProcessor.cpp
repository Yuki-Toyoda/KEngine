#include "PostProcessor.h"
#include "Engine/Base/DirectXCommon.h"

void PostProcessor::Init()
{
	// ポストプロセス初期化
	bloom_.Init();		// ブルーム
	outLine_.Init();	// アウトライン
	gaussian_.Init();	// ガウシアンフィルター
	radialBlur_.Init();	// ラジアルブラー
	vignette_.Init();	// ビネット
	grayScale_.Init();	// グレースケール
	hsvFilter_.Init();	// HSVフィルター

	// シェーダー生成開始
	CreateShaderFile();
}

void PostProcessor::Update()
{
	// ポストプロセスの更新を行う
	bloom_.Update();		// ブルーム
	outLine_.Update();		// アウトライン
	gaussian_.Update();		// ガウシアンフィルター
	radialBlur_.Update();	// ラジアルブラー
	vignette_.Update();		// ビネット
	grayScale_.Update();	// グレースケール
	hsvFilter_.Update();	// HSVフィルター
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
SamplerState gPointSampler : register(s1);

)";

	// ルートシグネチャのパラメータ生成
	root_.Init();
	root_.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);				// 汎用データ
	root_.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_PIXEL); // テクスチャデータ
	root_.CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_PIXEL); // 深度マップ
	root_.CreateSampler(0,
		D3D12_SHADER_VISIBILITY_PIXEL,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);																		// サンプラー
	root_.CreateSampler(1, 
		D3D12_SHADER_VISIBILITY_PIXEL, 
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	); // 深度テクスチャ用サンプラー

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
	output.a = 1.0f;

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
	pso_.Init(root_.GetRootSignature(), dxc, PSO::PSOType::Vertex)
		.SetDepthStencilState(false, true)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/PostProcess/PostProcess.PS.hlsl")
		.Build(device->GetDevice());
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
		bloom_.DisplayImGui();		// ブルーム
		outLine_.DisplayImGui();	// アウトライン
		gaussian_.DisplayImGui();	// ガウシアンフィルター
		radialBlur_.DisplayImGui();	// ラジアルブラー
		vignette_.DisplayImGui();	// ビネット
		grayScale_.DisplayImGui();	// グレースケール
		hsvFilter_.DisplayImGui();	// hsvフィルター

		// シェーダーの再読み込み
		if (ImGui::Button("Reload Shader")) {
			CreateShaderFile();
		}

		// ツリーノード終了
		ImGui::TreePop();
	}
}

void PostProcessor::CopyAllParameters(const PostProcessor& pp)
{
	// ポストプロセスパラメータのコピー
	this->bloom_.threshold_ = pp.bloom_.threshold_;
	this->outLine_.threshold_ = pp.outLine_.threshold_;
	this->outLine_.color_ = pp.outLine_.color_;
	this->gaussian_.intensity_ = pp.gaussian_.intensity_;
	this->radialBlur_.data_ = pp.radialBlur_.data_;
	this->vignette_.intensity_ = pp.vignette_.intensity_;
	this->grayScale_.intensity_ = pp.grayScale_.intensity_;
	this->hsvFilter_.hsv_ = pp.hsvFilter_.hsv_;
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
	result.push_back(&bloom_);		// ブルーム
	result.push_back(&outLine_);	// アウトライン
	result.push_back(&gaussian_);	// ガウシアンフィルター
	result.push_back(&radialBlur_);	// ラジアルブラー
	result.push_back(&vignette_);	// ビネット
	result.push_back(&grayScale_);	// グレースケール
	result.push_back(&hsvFilter_);	// HSVフィルター
	// 配列を返還する
	return result;
}
