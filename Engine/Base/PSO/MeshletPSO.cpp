#include "MeshletPSO.h"

void MeshletPSO::Init(ID3D12Device2* device, ID3D12RootSignature* signature, DXC* dxc, std::wstring vs, std::wstring ps, int blendType, bool isWriteDSV, UINT wire)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// シェーダー用のバイナリオブジェクト生成
	Microsoft::WRL::ComPtr<IDxcBlob> meshBlob = CreateMeshShader(dxc, vs); // 頂点シェーダー
	Microsoft::WRL::ComPtr<IDxcBlob> pixelBlob = CreatePixelShader(dxc, ps); // ピクセルシェーダー

	// パイプラインステートの設定を行う
	D3DX12_MESH_SHADER_PIPELINE_STATE_DESC graphicPipelineStateDesc{}; // パイプラインステート設定用構造体
	graphicPipelineStateDesc.pRootSignature = signature; // RootSignature取得
	graphicPipelineStateDesc.BlendState = SettingBlendState(blendType); // ブレンド設定
	graphicPipelineStateDesc.RasterizerState = SettingRasterizerDesc(wire); // ラスタライザ設定
	graphicPipelineStateDesc.DepthStencilState = SettingDepthStencilState(isWriteDSV); // 深度ステンシルビュー設定
	graphicPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット設定
	// シェーダーの取得
	graphicPipelineStateDesc.MS = { meshBlob->GetBufferPointer(), meshBlob->GetBufferSize() }; // 頂点シェーダー
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

	// パイプラインの型を変更
	auto PSOStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(graphicPipelineStateDesc);

	// パイプラインの設定を対応したものに変換する
	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
	streamDesc.pPipelineStateSubobjectStream = &PSOStream;
	streamDesc.SizeInBytes = sizeof(PSOStream);

	// 設定を元に実際に生成を行う
	result = device->CreatePipelineState(&streamDesc,
		IID_PPV_ARGS(&state_));
	// 生成出来ているかを確認する
	assert(SUCCEEDED(result));
}

IDxcBlob* MeshletPSO::CreateMeshShader(DXC* dxc, std::wstring ms)
{
	// 生成シェーダー格納用
	IDxcBlob* shaderBlob = nullptr;
	// 引数パスのシェーダーをコンパイルする
	shaderBlob = CompileShader(ms.c_str(), L"ms_6_5", dxc);
	assert(shaderBlob != nullptr); // コンパイル確認

	// 生成したシェーダーを返す
	return shaderBlob;
}
