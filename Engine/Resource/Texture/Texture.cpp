#include "Texture.h"
#include "../../Base/DirectXCommon.h"
#include "../../Base/Command/CommandManager.h"

Texture::Texture(CommandManager* manager, const std::string filePath)
{
	// テクスチャ情報のロード
	Load(filePath);
	// インデックス情報の取得
	index_ = manager->createTextureResource(mipImages_);
}

void Texture::Load(const std::string& filePath)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// テクスチャファイルを読み込み
	DirectX::ScratchImage image{};										   // テクスチャ格納用
	std::wstring filePathW = Debug::ConvertString(filePath);			   // wString形式に変換
	result = DirectX::LoadFromWICFile(
		filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image); // テクスチャをCPUで読み込む
	assert(SUCCEEDED(result));											   // 読み込み確認

	// ミップマップ生成
	result = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages_); // 生成
	assert(SUCCEEDED(result));																					 // 生成確認
}
