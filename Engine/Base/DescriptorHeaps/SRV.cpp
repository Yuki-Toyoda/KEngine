#include "SRV.h"
#include "../../../Externals/DirectXTex/DirectXTex.h"

using namespace Microsoft::WRL;

// コンフィグデータを短縮形で受け取れるように設定
#define MaxBuf    KEngine::Config::Rendering::kMaxBuffer  // 構造体バッファ最大数
#define MaxTex    KEngine::Config::Rendering::kMaxTexture // 読み込み可能テクスチャ最大数

#define TexOffset MaxBuf								  // 読み込み可能テクスチャ最大数

SRV::SRV(ID3D12Device2* device) :
	IDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, MaxBuf + MaxTex, MaxBuf),
	texIndexList_(MaxTex, TexOffset) // 構造体バッファの最大数分オフセットを指定して作成
{}

void SRV::Init()
{
	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kSize_, true);
}

SRVInfo SRV::RegisterStructuredBuffer(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc)
{
	// 返還用
	SRVInfo info;

	// 設定を引数からコピー
	info.desc_ = desc;

	// 空のインデックスを取得する
	info.index_ = indexList_.UseEmpty();
	// Viewを設定
	info.SetView(this);

	// SRVの生成を行う
	device_->CreateShaderResourceView(resource, &info.desc_, info.cpuView_);

	// 生成後、情報構造体を返す
	return info;
}

SRVInfo SRV::RegisterTexture(ID3D12Resource* resource, const DirectX::ScratchImage& mipImages)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 返還用
	SRVInfo info;

	// Meta情報の取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// 全MipMap分ループ
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; mipLevel++) {
		// MipLevelを指定して各Imagaeを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		result = resource->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		// 成功確認
		assert(SUCCEEDED(result));
	}

	// 読み込んだMetaDataを元にSRVの設定を行う
	info.desc_.Format				   = metadata.format;							// フォーマット設定
	info.desc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;  // シェーダーから読み取る
	info.desc_.ViewDimension		   = D3D12_SRV_DIMENSION_TEXTURE2D;				// テクスチャとして扱う
	info.desc_.Texture2D.MipLevels	   = UINT(metadata.mipLevels);					// ミップレベル指定

	// 空のインデックスを取得する
	info.index_ = texIndexList_.UseEmpty();
	// Viewを設定
	info.SetView(this);

	// SRVの生成を行う
	device_->CreateShaderResourceView(resource, &info.desc_, info.cpuView_);

	// 生成後、情報構造体を返す
	return info;
}

SRVInfo SRV::RegisterDepthTexture(ID3D12Resource* resource)
{
	// 返還用
	SRVInfo info;

	// 深度テクスチャ用のSRVの設定を行う
	info.desc_.Format				   = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	info.desc_.ViewDimension		   = D3D12_SRV_DIMENSION_TEXTURE2D;
	info.desc_.Texture2D.MipLevels	   = 1;
	info.desc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// SRV上の空きインデックスを取得する
	info.index_ = texIndexList_.UseEmpty();
	// Viewを設定
	info.SetView(this);

	// SRVの生成を行う
	device_->CreateShaderResourceView(resource, &info.desc_, info.cpuView_);

	// 生成後情報構造体を返す
	return info;
}

SRVInfo SRV::RegisterRenderResource(ID3D12Resource* resource, const int width, const int height)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 返還用
	SRVInfo info;

	// レンダーリソース関連のあれこれを求める
	const UINT pixelCount = width * height;	// 画素数を求める
	const UINT rowPitch	  = sizeof(UINT) * width; // 画像横一列分のサイズを求める
	const UINT depthPitch = rowPitch * height;	  // 画像サイズを求める
	
	// 画像の保存用の配列を生成する
	UINT* img = new UINT[pixelCount];
	// 全体を緑色で初期化する
	for (UINT i = 0; i < pixelCount; i++) {
		img[i] = 0xFF00FF00;
	}

	// TextureBufferに転送する
	result = resource->WriteToSubresource(
		0, 
		nullptr, 
		img, 
		rowPitch, 
		depthPitch);
	// 転送成否確認
	assert(SUCCEEDED(result));

	// SRVの設定
	info.desc_.Format				   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;			// フォーマット設定
	info.desc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // シェーダーから読み取る
	info.desc_.ViewDimension		   = D3D12_SRV_DIMENSION_TEXTURE2D;			   // テクスチャとして扱う
	info.desc_.Texture2D.MipLevels	   = 1;										   // ミップレベル指定

	// SRVの空きインデックスを取得する
	info.index_ = texIndexList_.UseEmpty();
	// アドレスを取得
	info.SetView(this);

	// SRVの生成を行う
	device_->CreateShaderResourceView(resource, &info.desc_, info.cpuView_);

	// 情報構造体を返す
	return info;
}

SRVInfo SRV::CreateImGuiSpace()
{
	// 返還用
	SRVInfo info;

	// 空のインデックスを取得する
	info.index_ = indexList_.UseEmpty();
	// Viewを設定
	info.SetView(this);

	// 情報構造体を返す
	return info;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRV::GetFirstTexView()
{
	// オフセットを元に、アドレスを返す
	return GetGPUHandle(TexOffset);
}

