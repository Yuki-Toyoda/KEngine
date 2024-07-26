#include "PSO.h"
#include "../../Debug/Debug.h"

#include <cassert>

PSO& PSO::Init(ID3D12RootSignature* signature, DXC* dxc, PSOType type)
{
	// 使用する設定用構造体の種類を特定
	type_ = type;

	// 設定用構造体の参照を取得
	D3DX12_MESH_SHADER_PIPELINE_STATE_DESC& mDesc = desc_.mesh;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC&		vDesc = desc_.vertex;
	D3D12_COMPUTE_PIPELINE_STATE_DESC&		cDesc = desc_.compute;

	// 種類によって処理を切り替え
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// ルートシグネチャをセット
		mDesc.pRootSignature = signature;
		// DXCもセットする
		dxc_ = dxc;

		// デフォルトのパラメーターをセットする
		mDesc.pRootSignature = signature;									 // ルートシグネチャのセット
		mDesc.BlendState = SettingBlendState(0);								 // ブレンド設定
		mDesc.RasterizerState = SettingRasterizerDesc();						 // ラスタライザ設定
		mDesc.DepthStencilState = SettingDepthStencilState(1, 1);			 // DSVの設定を行う
		mDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;						 // DSVのフォーマット設定
		mDesc.NumRenderTargets = 1;											 // 書き込むRTVの数
		mDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				 // RTVのフォーマット設定
		mDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 出力形状設定
		mDesc.SampleDesc.Count = 1;											 // サンプラー数
		mDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;						 // マスク設定

		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// ルートシグネチャをセット
		vDesc.pRootSignature = signature;
		// DXCもセットする
		dxc_ = dxc;

		// デフォルトのパラメーターをセットする
		vDesc.pRootSignature = signature;									 // ルートシグネチャのセット
		vDesc.BlendState = SettingBlendState(0);								 // ブレンド設定
		vDesc.RasterizerState = SettingRasterizerDesc();						 // ラスタライザ設定
		vDesc.DepthStencilState = SettingDepthStencilState(1, 1);			 // DSVの設定を行う
		vDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;						 // DSVのフォーマット設定
		vDesc.NumRenderTargets = 1;											 // 書き込むRTVの数
		vDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				 // RTVのフォーマット設定
		vDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 出力形状設定
		vDesc.SampleDesc.Count = 1;											 // サンプラー数
		vDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;						 // マスク設定
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ルートシグネチャをセット
		cDesc.pRootSignature = signature;
		// DXCもセットする
		dxc_ = dxc;

		// ルートシグネチャのセット
		cDesc.pRootSignature = signature;

		break;
	default: // それ以外の場合

		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);

		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetBlendState(int state)
{
	// ブレンドステートの設定を行う
	D3D12_BLEND_DESC blendDesc{};
	
	// ブレンド設定を行う
	blendDesc = SettingBlendState(state);

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// ブレンド設定をセット
		desc_.mesh.BlendState = blendDesc;
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// ブレンド設定をセット
		desc_.vertex.BlendState = blendDesc;
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set BlendState.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}
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

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// ラスタライザ設定をセット
		desc_.mesh.RasterizerState = rasterrizerDesc;
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// ラスタライザ設定をセット
		desc_.vertex.RasterizerState = rasterrizerDesc;
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set RasterizerState.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetMeshShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ms_6_5"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// シェーダーをセット
		desc_.mesh.MS = { blob->GetBufferPointer(), blob->GetBufferSize() };
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// ログに出力
		Debug::Log("Vertex can't set MeshShader.\n");
		// 停止させる
		assert(false);
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set MeshShader.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetPixelShader(std::string filePath)
{
	// コンパイル結果確認用
	IDxcBlob* mBlob = nullptr;
	IDxcBlob* vBlob = nullptr;

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// シェーダーコンパイルを行う
		mBlob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ps_6_5"); // シェーダーコンパイルを行う
		assert(mBlob != nullptr);											    // コンパイル成否確認

		// シェーダーをセット
		desc_.mesh.PS = { mBlob->GetBufferPointer(), mBlob->GetBufferSize() };
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		vBlob = dxc_->CompileShader(Debug::ConvertString(filePath), L"ps_6_0"); // シェーダーコンパイルを行う
		assert(vBlob != nullptr);											   // コンパイル成否確認

		// シェーダーをセット
		desc_.vertex.PS = { vBlob->GetBufferPointer(), vBlob->GetBufferSize() };
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set PixelShader.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetTaskShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"as_6_5"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// シェーダーをセット
		desc_.mesh.AS = { blob->GetBufferPointer(), blob->GetBufferSize() };
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// ログに出力
		Debug::Log("Vertex can't set TaskShader.\n");
		// 停止させる
		assert(false);
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set TaskShader.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetVertexShader(std::string filePath)
{
	// シェーダーコンパイルを行う
	IDxcBlob* blob = nullptr;											   // コンパイル結果確認用
	blob = dxc_->CompileShader(Debug::ConvertString(filePath), L"vs_6_0"); // シェーダーコンパイルを行う
	assert(blob != nullptr);											   // コンパイル成否確認

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// ログに出力
		Debug::Log("mesh can't set VertexShader.\n");
		// 停止させる
		assert(false);
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// シェーダーをセット
		desc_.vertex.VS = { blob->GetBufferPointer(), blob->GetBufferSize() };
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set VertexShader.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetDepthStencilState(bool writeDSV, bool enableMask)
{
	// DSVの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// DSVの設定を行う
	depthStencilDesc = SettingDepthStencilState(writeDSV, enableMask);

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// DSVの設定をセット
		desc_.mesh.DepthStencilState = depthStencilDesc;
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// DSVの設定をセット
		desc_.vertex.DepthStencilState = depthStencilDesc;
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set DepthStencilState.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

	// PSO自身を返す
	return *this;
}

PSO& PSO::SetDSVFormat(DXGI_FORMAT format)
{
	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// フォーマットをセット
		desc_.mesh.DSVFormat = format;
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// フォーマットをセット
		desc_.vertex.DSVFormat = format;
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// ログに出力
		Debug::Log("Compute can't set DSVFormat.\n");
		// 停止させる
		assert(false);
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}
	// PSO自身を返す
	return *this;
}

void PSO::Build(ID3D12Device2* device)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// PSOの型を指定形式に変換
	auto PSOStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(desc_.mesh);

	// PSOの種類によって処理を変更する
	switch (type_)
	{
	case PSO::PSOType::Mesh: // メッシュシェーダー
		// PSOの設定を対応したものに変換する
		D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
		streamDesc.pPipelineStateSubobjectStream = &PSOStream;
		streamDesc.SizeInBytes = sizeof(PSOStream);

		// 設定を元に実際に生成を行う
		result = device->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&state_));
		break;
	case PSO::PSOType::Vertex: // 頂点シェーダー
		// 設定を元に生成を行う
		result = device->CreateGraphicsPipelineState(&desc_.vertex, IID_PPV_ARGS(&state_));
		break;
	case PSO::PSOType::Compute: // 計算シェーダー
		// 設定を元に生成を行う
		result = device->CreateComputePipelineState(&desc_.compute, IID_PPV_ARGS(&state_));
		break;
	default: // それ以外
		// ログに出力
		Debug::Log("Empty PSO type.\n");
		// 停止させる
		assert(false);
		break;
	}

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

D3D12_DEPTH_STENCIL_DESC PSO::SettingDepthStencilState(bool isWriteDSV, bool enableMask)
{
	// デプスステンシルビューの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// 深度の機能を有効
	depthStencilDesc.DepthEnable = isWriteDSV;
	// 書き込み設定
	if (enableMask) {
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}
	else {
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	// 比較関数はlessEqual 近ければ描画
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// 設定を返す
	return depthStencilDesc;
}
