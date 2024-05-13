#include "PSO.h"

PSO& PSO::Init(ID3D12RootSignature* signature, DXC* dxc)
{
	// ルートシグネチャをセット
	desc_.pRootSignature = signature;
	// DXCもセットする
	dxc_ = dxc;

	// デフォルトのパラメーターをセットする
	desc_.pRootSignature		= signature;							  // ルートシグネチャのセット
	desc_.BlendState			= SettingBlendState(0);					  // ブレンド設定
	desc_.RasterizerState		= SettingRasterizerDesc();				  // ラスタライザ設定
	desc_.DepthStencilState		= SettingDepthStencilState(1);			  // DSVの設定を行う
	desc_.DSVFormat				= DXGI_FORMAT_D24_UNORM_S8_UINT;		  // DSVのフォーマット設定
	desc_.NumRenderTargets		= 1;									  // 書き込むRTVの数
	desc_.RTVFormats[0]			= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		  // RTVのフォーマット設定
	desc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 出力形状設定
	desc_.SampleDesc.Count		= 1;									  // サンプラー数
	desc_.SampleMask			= D3D12_DEFAULT_SAMPLE_MASK;			  // マスク設定

	// PSO自身を返す
	return *this;
}

PSO& PSO::VertInit(ID3D12RootSignature* signature, DXC* dxc)
{
	// ルートシグネチャをセット
	vertDesc_.pRootSignature = signature;
	// DXCもセットする
	dxc_ = dxc;

	// デフォルトのパラメーターをセットする
	vertDesc_.pRootSignature = signature;									  // ルートシグネチャのセット
	vertDesc_.BlendState = SettingBlendState(0);							  // ブレンド設定
	vertDesc_.RasterizerState = SettingRasterizerDesc();					  // ラスタライザ設定
	vertDesc_.DepthStencilState = SettingDepthStencilState(0);				  // DSVの設定を行う
	vertDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;					  // DSVのフォーマット設定
	vertDesc_.NumRenderTargets = 1;											  // 書き込むRTVの数
	vertDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				  // RTVのフォーマット設定
	vertDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 出力形状設定
	vertDesc_.SampleDesc.Count = 1;											  // サンプラー数
	vertDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;						  // マスク設定

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetBlendState(int state)
{
	// ブレンドステートの設定を行う
	D3D12_BLEND_DESC blendDesc{};
	
	// ブレンド設定を行う
	blendDesc = SettingBlendState(state);

	// ブレンド設定をセット
	desc_.BlendState = blendDesc;

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetRasterizerState(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode)
{
	// ラスタライザステートの設定を行う
	D3D12_RASTERIZER_DESC rasterrizerDesc{};
	// 背面カリング
	rasterrizerDesc.CullMode = cullMode;

	// 三角形の中を塗りつぶす
	rasterrizerDesc.FillMode = fillMode;

	// ラスタライザ設定をセット
	desc_.RasterizerState = rasterrizerDesc;

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetMeshShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ms_6_5"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// シェーダーをセット
	desc_.MS = { blob->GetBufferPointer(), blob->GetBufferSize() };

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetPixelShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ps_6_5"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// シェーダーをセット
	desc_.PS = { blob->GetBufferPointer(), blob->GetBufferSize() };

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetVertexShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"vs_6_0"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// シェーダーをセット
	vertDesc_.VS = { blob->GetBufferPointer(), blob->GetBufferSize() };

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetVertPixelShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ps_6_0"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// シェーダーをセット
	vertDesc_.PS = { blob->GetBufferPointer(), blob->GetBufferSize() };

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetDepthStencilState(bool writeDSV)
{
	// DSVの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// DSVの設定を行う
	depthStencilDesc = SettingDepthStencilState(writeDSV);

	// DSVの設定をセット
	desc_.DepthStencilState = depthStencilDesc;

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetDSVFormat(DXGI_FORMAT format)
{
	// フォーマットをセット
	desc_.DSVFormat = format;
	// PSO自身を返す
	return *this;
}

void PSO::Build(ID3D12Device2* device)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// PSOの型を指定形式に変換
	auto PSOStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(desc_);
	// PSOの設定を対応したものに変換する
	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
	streamDesc.pPipelineStateSubobjectStream = &PSOStream;
	streamDesc.SizeInBytes = sizeof(PSOStream);

	// 設定を元に実際に生成を行う
	result = device->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&state_));
	// 生成成否確認
	assert(SUCCEEDED(result));
}

void PSO::VertBuild(ID3D12Device2* device)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 設定を元に実際に生成を行う
	result = device->CreateGraphicsPipelineState(&vertDesc_, IID_PPV_ARGS(&state_));
	// 生成成否確認
	assert(SUCCEEDED(result));
}

D3D12_BLEND_DESC PSO::SettingBlendState(int state)
{
	// ブレンドステートの設定を行う
	D3D12_BLEND_DESC blendDesc{};
	// 全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	// ノーマルブレンドの設定
	blendDesc.RenderTarget[0].BlendEnable = true; // ブレンド有効
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	// 渡されたステート情報を元に設定
	switch (state)
	{
	case 0: // 通常の設定
		// そのまま抜ける
		break;
	case 1: // 加算の設定
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break; // 減算の設定
	case 2:
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break; // 乗算の設定
	case 3:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break; // スクリーンの設定
	case 4:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	default: // それ以外の場合は処理なしで抜ける
		break;
	}

	// ブレンド設定を返す
	return blendDesc;
}

D3D12_RASTERIZER_DESC PSO::SettingRasterizerDesc()
{
	// ラスタライザステートの設定を行う
	D3D12_RASTERIZER_DESC rasterrizerDesc{};
	// 背面カリング
	rasterrizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	// 三角形の中を塗りつぶす
	rasterrizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// 設定を返す
	return rasterrizerDesc;
}

D3D12_DEPTH_STENCIL_DESC PSO::SettingDepthStencilState(bool isWriteDSV)
{
	// デプスステンシルビューの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// 深度の機能を有効
	depthStencilDesc.DepthEnable = isWriteDSV;
	// 書き込む
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はlessEqual 近ければ描画
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// 設定を返す
	return depthStencilDesc;
}
