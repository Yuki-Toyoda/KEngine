#include "IRootSignature.h"

void IRootSignature::PreInit(const int& rootParameterCount)
{
	// ルートシグネチャの設定を行う
	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // フラッグはなし
	descriptionRootSignature_.pParameters = rootParameters_.data();   // 設定用構造体にrootParameterを登録
	descriptionRootSignature_.NumParameters = rootParameterCount;	  // パラメータは指定した数
}

void IRootSignature::CreateCBVParameter(const int& ShaderRegisterCount, const D3D12_SHADER_VISIBILITY& visibility)
{
	// 新規パラメータを生成
	D3D12_ROOT_PARAMETER newParameter;

	// 定数バッファ用パラメータを生成
	newParameter.ParameterType			   = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	newParameter.ShaderVisibility		   = visibility;					 // 指定したVisibilityでパラメータを設定
	newParameter.Descriptor.ShaderRegister = ShaderRegisterCount;			 // レジスタ番号を指定した番号とバインド

	// 生成したパラメータを追加
	rootParameters_.push_back(newParameter);
}

void IRootSignature::CreateDescriptorTableParameter(const int& ShaderRegisterCount, const D3D12_SHADER_VISIBILITY& visibility, const D3D12_DESCRIPTOR_RANGE_TYPE& type, const int& numDescriptor)
{
	// 新規パラメータを生成
	D3D12_ROOT_PARAMETER newParameter;

	// 配列用の設定構造体を定義
	D3D12_DESCRIPTOR_RANGE descRange[1]			   = {};								   // DescriptorRangeを作成
	descRange[0].BaseShaderRegister				   = ShaderRegisterCount;				   // レジスタ番号を指定した番号とバインド
	descRange[0].NumDescriptors					   = numDescriptor;						   // 数は1つ
	descRange[0].RegisterSpace					   = 0;									   // 0に設定
	descRange[0].RangeType						   = type;								   // 指定したパラメータで設定
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// 定数バッファ用パラメータを生成
	newParameter.ParameterType						 = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	newParameter.ShaderVisibility					 = visibility;								   // 指定したVisibilityでパラメータを設定
	newParameter.DescriptorTable.pDescriptorRanges   = descRange;								   // ディスクリプタのレンジを設定
	newParameter.DescriptorTable.NumDescriptorRanges = _countof(descRange);						   // テーブルのサイズを設定

	// 生成したパラメータを追加
	rootParameters_.push_back(newParameter);
}

void IRootSignature::CreateRootSignature()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// シリアライズを行う
	ID3DBlob* signatureBlob = nullptr; // シリアライズ後のバイナリオブジェクト
	ID3DBlob* errorBlob = nullptr;     // エラーログを出すためのバイナリオブジェクト
	// ルートシグネチャ用にシリアライズ
	result = D3D12SerializeRootSignature(&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	// 生成に失敗した場合
	if (FAILED(result)) {
		// ログを出力
		Debug::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		// 停止
		assert(false);
	}
	// バイナリを元にルートシグネチャを生成
	result = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)); // 生成
	assert(SUCCEEDED(result));                                                                                                                  // 生成確認

	// 使わないリソースを解放
	signatureBlob->Release();
	errorBlob->Release();
}

void IRootSignature::Init()
{
	// 基底クラスでは処理を記述しない
}
