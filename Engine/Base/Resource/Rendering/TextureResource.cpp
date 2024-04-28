#include "TextureResource.h"
#include "../../../Debug/Debug.h"

void TextureResource::Init(DirectXDevice* device, HeapManager* heaps, std::string filePath)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// テクスチャを読み込む
	Load(filePath);

	// メタデータの読み込み
	const DirectX::TexMetadata metadata(mipImages_.GetMetadata());
	
	// 解像度を取得
	width_  = static_cast<int>(metadata.width);  // 横幅
	height_ = static_cast<int>(metadata.height); // 縦幅

	// Reourceの設定
	desc_.Width			   = UINT16(metadata.width);						   // 解像度横幅
	desc_.Height		   = UINT16(metadata.height);						   // 解像度縦幅
	desc_.MipLevels		   = UINT16(metadata.mipLevels);					   // ミップレベル
	desc_.DepthOrArraySize = UINT16(metadata.arraySize);					   // 配列サイズ
	desc_.Format		   = metadata.format;							   // フォーマット設定
	desc_.SampleDesc.Count = 1;											   // カウント設定
	desc_.Dimension		   = D3D12_RESOURCE_DIMENSION(metadata.dimension); // ディメンション設定

	// 利用するヒープの設定
	properties_.Type				 = D3D12_HEAP_TYPE_CUSTOM;			   // 細かい設定を行う
	properties_.CPUPageProperty		 = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	properties_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;			   // プロセッサの近くに配置

	// リソースを生成する
	result = device->GetDevice()->CreateCommittedResource(
		&properties_,					   // ヒープ設定
		D3D12_HEAP_FLAG_NONE,			   // フラッグ設定
		&desc_,							   // リソース設定	
		D3D12_RESOURCE_STATE_GENERIC_READ, // 読み取り専用
		nullptr,						   // 使わないためnullptr
		IID_PPV_ARGS(&resource_)		   // 作成するResourceのポインタ
	);

	// 生成に失敗した場合停止させる
	assert(SUCCEEDED(result));

	// 登録したSRV上のインデックスを取得する
	info_ = heaps->srv()->RegisterTexture(resource_.Get(), mipImages_);
}

void TextureResource::Load(const std::string& filePath)
{
	// テクスチャ読み込みデータ一時格納用
	DirectX::ScratchImage image{};
	// 後に呼び出す関数ようにwstring形式に変換する
	std::wstring filePathW = Debug::ConvertString(filePath);
	// テクスチャを読み込む
	HRESULT result = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	// 読み込みに失敗した場合停止させる
	assert(SUCCEEDED(result));

	// ミップマップの作成
	result = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages_);
	// 生成に失敗した場合停止させる
	assert(SUCCEEDED(result));
}
