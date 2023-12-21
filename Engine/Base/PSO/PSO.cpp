#include "PSO.h"

void PSO::Init(ID3D12Device* device, ID3D12RootSignature* signature, DXC* dxc, std::wstring vs, std::wstring ps, int blendType, bool isWriteDSV = true, UINT wire = false)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// シェーダー用のバイナリオブジェクト生成
	Microsoft::WRL::ComPtr<IDxcBlob> vertexBlob = CreateVertexShader(dxc, vs); // 頂点シェーダー
	Microsoft::WRL::ComPtr<IDxcBlob> pixelBlob = CreatePixelShader(dxc, ps); // ピクセルシェーダー

	// パイプラインステートの設定を行う
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicPipelineStateDesc{}; // パイプラインステート設定用構造体
	graphicPipelineStateDesc.pRootSignature = signature; // RootSignature取得
	graphicPipelineStateDesc.BlendState = SettingBlendState(blendType); // ブレンド設定
	graphicPipelineStateDesc.RasterizerState = SettingRasterizerDesc(wire); // ラスタライザ設定
	graphicPipelineStateDesc.DepthStencilState = SettingDepthStencilState(isWriteDSV); // 深度ステンシルビュー設定
	graphicPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット設定
	// シェーダーの取得
	graphicPipelineStateDesc.VS = { vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize() }; // 頂点シェーダー
	graphicPipelineStateDesc.PS = { pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize() }; // ピクセルシェーダー

	// 書き込むRTVの情報
	graphicPipelineStateDesc.NumRenderTargets = 1;
	graphicPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジーのタイプ
	graphicPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 3角形

	// どのように画面に色を打ち込むかの設定
	graphicPipelineStateDesc.SampleDesc.Count = 1;
	graphicPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 設定を元に実際に生成を行う
	result = device->CreateGraphicsPipelineState(&graphicPipelineStateDesc,
		IID_PPV_ARGS(&state_));
	// 生成出来ているかを確認する
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

D3D12_RASTERIZER_DESC PSO::SettingRasterizerDesc(UINT wire)
{
	// ラスタライザステートの設定を行う
	D3D12_RASTERIZER_DESC rasterrizerDesc{};
	// 背面カリング
	rasterrizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	// フラグによって表示状態切り替え
	switch (wire)
	{
	case 0:
	default:
		// 三角形の中を塗りつぶす
		rasterrizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		break;
	case 1: 
		// ワイヤーフレームで描画
		rasterrizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	}

	// 設定を返す
	return rasterrizerDesc;
}

D3D12_DEPTH_STENCIL_DESC PSO::SettingDepthStencilState(bool isWriteDSV)
{
	// デプスステンシルビューの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// 深度の機能を有効
	depthStencilDesc.DepthEnable = true;
	// 書き込む
	if (isWriteDSV) {
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

IDxcBlob* PSO::CreateVertexShader(DXC* dxc, std::wstring vs)
{
	// 生成シェーダー格納用
	IDxcBlob* shaderBlob = nullptr;

	// 引数パスのシェーダーをコンパイルする
	shaderBlob = CompileShader(vs.c_str(), L"vs_6_0", dxc);
	assert(shaderBlob != nullptr); // コンパイル確認

	// 生成したシェーダーを返す
	return shaderBlob;
}

IDxcBlob* PSO::CreatePixelShader(DXC* dxc, std::wstring ps)
{
	// 生成シェーダー格納用
	IDxcBlob* shaderBlob = nullptr;
	// 引数パスのシェーダーをコンパイルする
	shaderBlob = CompileShader(ps.c_str(), L"ps_6_0", dxc);
	assert(shaderBlob != nullptr); // コンパイル確認

	// 生成したシェーダーを返す
	return shaderBlob;
}

IDxcBlob* PSO::CompileShader(const std::wstring& filePath, const wchar_t* profile, DXC* dxc)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// シェーダーコンパイルの開始をログに出す
	Debug::Log(Debug::ConvertString(std::format(L"Begin CompileShader, path:{}, profile{}\n", filePath, profile)));

	// HLSLファイルを読み込む
	IDxcBlobEncoding* shaderSource = nullptr;
	result = dxc->dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読み込み出来ているかを確認する
	assert(SUCCEEDED(result));

	// 読み込んだファイルの内容の設定を行う
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	// UTF-8形式の文字コードとして設定する
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	// コンパイルオプションの設定を行う
	LPCWSTR arguments[] = {
		filePath.c_str(), // コンパイル対象のhlslファイル名
		L"-E", L"main", // エントリーポイントの指定 基本的にmain以外にはしない
		L"-T", profile, // shaderProfileの設定
		L"-Zi", L"-Qembed_debug", // デバック用の情報埋め込み
		L"-Od", // 最適化を外す
		L"-Zpr", // メモリレイアウトは行優先
	};

	// 実際にシェーダーのコンパイルを行う
	IDxcResult* shaderResult = nullptr;
	result = dxc->dxcCompiler_->Compile(
		&shaderSourceBuffer,             // 読み込んだファイル
		arguments,                       // コンパイルオプション
		_countof(arguments),             // コンパイルオプションの数
		dxc->dxcIncludeHandler_.Get(),   // include が含まれた諸々
		IID_PPV_ARGS(&shaderResult)      // コンパイル結果
	);

	// ここで吐くエラーはコンパイルエラーではなく、dxcが起動できないなどの致命的なもの
	assert(SUCCEEDED(result));

	// 警告やエラーが出た場合にはログに出して停止させる
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		// ログに警告、エラー情報を出力する
		Debug::Log(shaderError->GetStringPointer());
		// 警告やエラーが出ている場合停止させる
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得する
	IDxcBlob* shaderBlob = nullptr;
	result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	// 取得出来たか確認する
	assert(SUCCEEDED(result));

	// 成功したことをログに出力
	Debug::Log(Debug::ConvertString(std::format(L"Compile Succeeded, path:{}, profile{}\n", filePath, profile)));

	// 使わないリソースの解放を行う
	shaderSource->Release();
	shaderResult->Release();

	// 実行用のバイナリを返す
	return shaderBlob;
}
